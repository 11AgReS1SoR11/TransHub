
#include <QDebug>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>

#include "TCPClient.hpp"
namespace TCP
{

TCPClient::TCPClient(QObject *parent) : QObject(parent)
{
    initialize();
}

TCPClient::TCPClient(port_t port, QObject *parent) : QObject(parent)
{
    initialize();
    connectToHost(port, QHostAddress::LocalHost);
}

TCPClient::~TCPClient()
{
    if (socket)
    {
        if(socket->isOpen())
        {
            socket->close();
        }
        socket->deleteLater();
    }
}

void TCPClient::initialize()
{
    // set timer
    messageTimer = new QTimer(this);
    messageTimer->setSingleShot(true);

    connect(this, &TCPClient::newMessage, this, &TCPClient::stopTimer);
    connect(messageTimer, &QTimer::timeout, this, &TCPClient::handleTimeout);

    // set socket
    socket = new QTcpSocket(this);

    connect(this, &TCPClient::newMessage, this, &TCPClient::displayMessage);
    connect(socket, &QTcpSocket::readyRead, this, &TCPClient::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &TCPClient::discardSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &TCPClient::displayError);
}

bool TCPClient::connectToHost(port_t port, const QHostAddress& address)
{
    if (!socket)
    {
        socket = new QTcpSocket(this);
    }

    socket->connectToHost(address, port);

    if (socket->waitForConnected())
    {
        qDebug() << "Connected to Server";
        return true;
    }

    qDebug() << QString("The following error occurred: %1.").arg(socket->errorString());
    return false;
}

void TCPClient::readSocket()
{
    qDebug() << "[TCPClient] readSocket";

    if (!isConnected())
    {
        qDebug() << "[TCPClient] No connection";
        return;
    }

    QByteArray buffer;
    QDataStream socketStream(socket);

    socketStream.startTransaction();
    socketStream >> buffer;

    if (!socketStream.commitTransaction()) // unsuccessful case
    {
        qDebug() << QString("[TCPClient] Waiting for more data to come..");
        return;
    }

    QString message = QString::fromStdString(buffer.toStdString());
    emit newMessage(message);
}

void TCPClient::disconnect() noexcept
{
    if (socket && socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->close();
        qDebug() << "[TCPClient] Disconnected!";
    }
    else
    {
        qDebug() << "[TCPClient] No connections";
    }
}

void TCPClient::discardSocket()
{
    qDebug() << "[TCPClient] discardSocket";

    socket->deleteLater();
    socket = nullptr;
}

void TCPClient::displayError(QAbstractSocket::SocketError socketError) const noexcept
{
    qDebug() << "[TCPClient] Error";

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
            qDebug() << QString("The following error occurred: %1.").arg(socket->errorString());
            break;
        }
    }
}

void TCPClient::sendMessage(const QString& msg)
{
    qDebug() << "[TCPClient] sending message...";

    if (!isConnected())
    {
        qDebug() << "[TCPClient] No connections";
        return;
    }

    QDataStream socketStream(socket);
    QByteArray byteArray = msg.toUtf8();

    socketStream << byteArray;

    restartTimer();
}

void TCPClient::displayMessage(const QString& msg) const noexcept
{
    qDebug() << "[TCPClient] displayMessage: " << msg;
}

void TCPClient::restartTimer()
{
    messageTimer->start(TCP::RESPONSE_TIMEOUT);
}

void TCPClient::stopTimer()
{
    messageTimer->stop();
}

void TCPClient::handleTimeout()
{
    qDebug() << "[TCPClient] Timeout: No messages received within the last minute. Disconnecting...";
    disconnect();
}

bool TCPClient::isConnected() const noexcept
{
    return (socket && socket->state() == QAbstractSocket::ConnectedState);
}

} // namespace TCP