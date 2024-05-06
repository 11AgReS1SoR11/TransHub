#ifndef TCPClient_HPP
#define TCPClient_HPP

#include <QAbstractSocket>
#include <QHostAddress>
#include <QMetaType>

#include "Constants.hpp"

class QTcpSocket; // forward declaration
class QTimer; // forward declaration
namespace TCP::Protocol
{
    class Proto; // forward declaration
}

namespace TCP
{

class TCPClient : public QObject
{
    Q_OBJECT

public:
    explicit TCPClient(QObject *parent = nullptr);
    explicit TCPClient(port_t port, QObject *parent = nullptr);
    ~TCPClient();

    bool connectToHost(port_t port, const QHostAddress& address = QHostAddress::LocalHost);
    bool isConnected() const noexcept;
    void sendMessage(const Protocol::Proto&);

signals:
    void newMessage(Protocol::Proto&);

public slots:
    void disconnect() noexcept;

private slots:
    void readSocket();
    void discardSocket();

    void displayError(QAbstractSocket::SocketError socketError) const noexcept;
    void displayMessage(const Protocol::Proto&) const noexcept;

    void restartTimer();
    void stopTimer();
    void handleTimeout();

private:
    void initialize();

    QTcpSocket* socket;
    QTimer* messageTimer;
};

} // namespace TCP

#endif // TCPClient_HPP