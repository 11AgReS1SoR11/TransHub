#ifndef TCPServer_HPP
#define TCPServer_HPP

#include <QMetaType>
#include <QAbstractSocket>
#include <QSet>

#include "Constants.hpp"

class QTcpServer; // forward declaration
class QTcpSocket; // forward declaration

namespace TCP
{

class TCPServer : public QObject
{
    Q_OBJECT

public:
    explicit TCPServer(port_t port, QObject *parent = nullptr);
    ~TCPServer();

    void sendMessage(tcp_id_t receiverid, const QString& msg);

signals:
    void newMessage(tcp_id_t clientId, const QString& msg);

private slots:
    void newConnection();

    void readSocket();
    void discardSocket();

    void displayError(QAbstractSocket::SocketError socketError);
    void displayMessage(tcp_id_t clientId, const QString& str) const noexcept;

private:
    static bool isConnected(QTcpSocket* socket) noexcept;
    void appendToSocketList(QTcpSocket* socket);
    void sendMessage(QTcpSocket* socket, const QString& str);

    QTcpServer* m_server;
    QSet<QTcpSocket*> connection_set;
};

} // namespace TCP

#endif // TCPServer_HPP
