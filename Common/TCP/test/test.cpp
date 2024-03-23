#include <QtTest/QtTest>

#include "TCPServer.hpp"
#include "TCPClient.hpp"

static constexpr TCP::port_t PORT1 = 8080;
static constexpr TCP::port_t PORT2 = 8081;
static constexpr TCP::port_t PORT3 = 8082;
static constexpr TCP::port_t PORT4 = 8083;
static constexpr TCP::port_t PORT5 = -1;

class TestClass : public QObject
{
    Q_OBJECT

private:
    TCP::TCPServer srv1{PORT2};
    TCP::TCPServer srv2{PORT3};
    TCP::TCPClient cli1;
    TCP::TCPClient cli2;

    void client_server_communication();

private slots:
    void test_connect();
    void test_data_transfer();
    void test_failure_connection();
    void test_reconnection();
    void test_timeout();
    void compareMessageFromServer(QString);
    void compareMessageFromClient(qintptr clientId, QString);
    void compareMessageFromClientTimeout(qintptr clientId, QString);
};

void TestClass::compareMessageFromServer(QString msg)
{
    QVERIFY(msg == QString("Helloy, client"));
}

void TestClass::compareMessageFromClient(qintptr clientId, QString msg)
{
    QVERIFY(msg == "Helloy, server");

    srv1.sendMessage(clientId, "Helloy, client");
}

void TestClass::compareMessageFromClientTimeout(qintptr, QString msg)
{
    QVERIFY(msg == "Helloy, server");
    QTest::qWait(TCP::RESPONSE_TIMEOUT + 1000);
}

void TestClass::client_server_communication()
{
    connect(&srv1, &TCP::TCPServer::newMessage, this, &TestClass::compareMessageFromClient);
    connect(&cli1, &TCP::TCPClient::newMessage, this, &TestClass::compareMessageFromServer);

    cli1.sendMessage("Helloy, server");
    QTest::qWait(100); // wait server response

    cli1.disconnect();
    QVERIFY(cli1.isConnected() == false);
}

void TestClass::test_connect()
{
    TCP::TCPServer server{PORT1};
    TCP::TCPClient client1;
    TCP::TCPClient client2;
    TCP::TCPClient client3;

    bool connected = client1.connectToHost(PORT1);
    QVERIFY(connected == true);

    connected = client2.connectToHost(PORT1);
    QVERIFY(connected == true);

    connected = client3.connectToHost(PORT1);
    QVERIFY(connected == true);

    QVERIFY(client1.isConnected() == true);
    QVERIFY(client2.isConnected() == true);
    QVERIFY(client3.isConnected() == true);

    client1.disconnect();
    client2.disconnect();
    client3.disconnect();

    QVERIFY(client1.isConnected() == false);
    QVERIFY(client2.isConnected() == false);
    QVERIFY(client3.isConnected() == false);
}

void TestClass::test_data_transfer()
{
    // use new port for every test case
    bool connected = cli1.connectToHost(PORT2);
    QVERIFY(connected == true);

    client_server_communication();
}

void TestClass::test_timeout()
{
    bool connected = cli2.connectToHost(PORT3);
    QVERIFY(connected == true);

    connect(&srv2, &TCP::TCPServer::newMessage, this, &TestClass::compareMessageFromClientTimeout);

    cli2.sendMessage("Helloy, server");
    QTest::qWait(100); // wait server response

    QVERIFY(cli2.isConnected() == false);
}

void TestClass::test_failure_connection()
{
    TCP::TCPServer server{PORT4};
    TCP::TCPClient client;

    client.disconnect(); // отключаюсь от неподключённого сервера

    bool connected = client.connectToHost(PORT5); // connect to unexisted server port
    QVERIFY(connected == false);

    client.disconnect(); // отключаюсь от неподключённого сервера

    client.sendMessage("Helloy, server"); // сообщение к неподключённому серверу
    QVERIFY(cli2.isConnected() == false);

    server.sendMessage(qintptr(), "Hi nobody"); // сообщение в никуда
}

void TestClass::test_reconnection()
{
    cli1.disconnect();
    QVERIFY(cli1.isConnected() == false);

    bool connected = cli1.connectToHost(PORT2);
    QVERIFY(connected == true);

    cli1.disconnect();
    QVERIFY(cli1.isConnected() == false);

    connected = cli1.connectToHost(PORT2);
    QVERIFY(connected == true);

    // message after reconnection
    client_server_communication();
}

QTEST_GUILESS_MAIN(TestClass)
#include "test.moc"
