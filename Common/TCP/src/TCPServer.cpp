#include <QDebug>
#include <QDataStream>
#include <QTcpServer>
#include <QTcpSocket>

#include "TCPServer.hpp"

namespace TCP
{

TCPServer::TCPServer(port_t port, QObject *parent) : QObject(parent)
{
    m_server = new QTcpServer(this);

    if(m_server->listen(QHostAddress::Any, port))
    {
       connect(this, &TCPServer::newMessage, this, &TCPServer::displayMessage);
       connect(m_server, &QTcpServer::newConnection, this, &TCPServer::newConnection);

       qDebug() << "[TCPServer] Server is listening on port" << port;
    }
    else
    {
        qDebug() << QString("[TCPServer] Unable to start the server on port %1: %2.").arg(port).arg(m_server->errorString());
    }
}

TCPServer::~TCPServer()
{
    foreach(QTcpSocket* socket, connection_set)
    {
        if (socket->isOpen())
        {
            socket->close();
        }
        socket->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();
}

void TCPServer::newConnection()
{
    qDebug() << "[TCPServer] New connection";

    while(m_server->hasPendingConnections())
    {
        appendToSocketList(m_server->nextPendingConnection());
    }
}

void TCPServer::appendToSocketList(QTcpSocket* socket)
{
    connection_set.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &TCPServer::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &TCPServer::discardSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &TCPServer::displayError);

    qDebug() << QString("[TCPServer] Client[ID=%1] with port:%2 has just entered the room").arg(socket->socketDescriptor()).arg(socket->peerPort());
}

void TCPServer::readSocket()
{
    qDebug() << "[TCPServer] readSocket";

    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray buffer;
    QDataStream socketStream(socket);

    socketStream.startTransaction();
    socketStream >> buffer;

    if (!socketStream.commitTransaction()) // unsuccessful case
    {
        qDebug() << QString("[TCPServer] Client[ID=%1] Waiting for more data to come..").arg(socket->socketDescriptor());
        return;
    }

    QString message = QString::fromStdString(buffer.toStdString());
    emit newMessage(socket->socketDescriptor(), message);
}

void TCPServer::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = connection_set.find(socket);

    if (it != connection_set.end())
    {
        qDebug() << QString("[TCPServer] A Client[ID=%1] has just left the room").arg(socket->socketDescriptor());

        socket->deleteLater();
        connection_set.remove(*it);
    }
}

void TCPServer::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "[TCPServer] Error";

    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
        {
            qDebug() << "Error: RemoteHostClosedError";
            break;
        }
        case QAbstractSocket::HostNotFoundError:
        {
            qDebug() << "The host was not found. Please check the host name and port settings.";
            break;
        }
        case QAbstractSocket::ConnectionRefusedError:
        {
            qDebug() << "The connection was refused by the peer. Make sure QTCPServer is running, and check that the host name and port settings are correct.";
            break;
        }
        default:
        {
            QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
            qDebug() << QString("The following error occurred: %1.").arg(socket->errorString());
            break;
        }
    }
}

void TCPServer::sendMessage(tcp_id_t receiverid, const QString& msg)
{
    qDebug() << QString("[TCPServer] sending message to Client[ID=%1]").arg(receiverid);

    bool sended = false;
    foreach(QTcpSocket* socket, connection_set)
    {
        if (socket->socketDescriptor() == receiverid)
        {
            sendMessage(socket, msg);
            sended = true;
            break;
        }
    }

    if (!sended)
        qDebug() << QString("[TCPServer] No connection with Client[ID=%1]").arg(receiverid);
}

void TCPServer::sendMessage(QTcpSocket* socket, const QString& str)
{
    if(!isConnected(socket))
    {
        qDebug() << QString("[TCPServer] No connection with Client[ID=%1]").arg(socket->socketDescriptor());
        return;
    }

    QDataStream socketStream(socket);

    QByteArray byteArray = str.toUtf8();
    socketStream << byteArray;
}

void TCPServer::displayMessage(tcp_id_t clientId, const QString& str) const noexcept
{
    qDebug() << QString("Client[ID=%1]: %2").arg(clientId).arg(str);
}

bool TCPServer::isConnected(QTcpSocket* socket) noexcept
{
    return (socket && socket->state() == QAbstractSocket::ConnectedState);
}

} // namespace TCP