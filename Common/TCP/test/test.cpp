#include <QtTest/QtTest>

#include "TCPServer.hpp"
#include "TCPClient.hpp"
#include "TCPProto.hpp"

static constexpr TCP::port_t PORT1 = 8080;
static constexpr TCP::port_t PORT2 = 8081;
static constexpr TCP::port_t PORT3 = 8082;
static constexpr TCP::port_t PORT4 = 8083;
static constexpr TCP::port_t PORT5 = -1;
static Mtx::Matrix<int> matrix = {{1, 2, 2, 8},
                                  {3, 1, 1, 5},
                                  {4, 2, 2, 6},
                                  {5, 2, 3, 5}};
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
    void test_protocol_matrix_int();
    void test_protocol_matrix_double();
    void test_protocol_string();
    void test_connect();
    void test_data_transfer();
    void test_failure_connection();
    void test_reconnection();
    void test_timeout();
    void compareMessageFromServer(TCP::Protocol::Proto);
    void compareMessageFromClient(qintptr clientId, TCP::Protocol::Proto);
    void compareMessageFromClientTimeout(qintptr clientId, TCP::Protocol::Proto);
};

void TestClass::test_protocol_matrix_int()
{
    Mtx::Matrix<int> matrixInt = {{1, 2, 2, 8},
                                  {3, 1, 1, 5},
                                  {4, 2, 2, 6},
                                  {5, 2, 3, 5}};

    TCP::Protocol::Proto proto{&matrixInt};

    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out << proto;

    // Восстановление данных из QByteArray
    TCP::Protocol::Proto protoRestored;
    QDataStream in(&byteArray, QIODevice::ReadOnly);
    in >> protoRestored;

    QVERIFY(!protoRestored.getString());
    QVERIFY(!protoRestored.getMatrixDouble());

    auto matrix_ptr = protoRestored.getMatrixInt();
    QVERIFY(matrix_ptr);
    QVERIFY(*matrix_ptr == matrixInt);
}

void TestClass::test_protocol_matrix_double()
{
    Mtx::Matrix<double> matrixDouble = {{1.01, 2.01, 2.01, 8.01},
                                        {3.01, 1.01, 1.01, 5.01},
                                        {4.01, 2.01, 2.01, 6.01},
                                        {5.01, 2.01, 3.01, 5.01}};

    TCP::Protocol::Proto proto{&matrixDouble};

    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out << proto;

    // Восстановление данных из QByteArray
    TCP::Protocol::Proto protoRestored;
    QDataStream in(&byteArray, QIODevice::ReadOnly);
    in >> protoRestored;

    QVERIFY(!protoRestored.getString());
    QVERIFY(!protoRestored.getMatrixInt());

    auto matrix_ptr = protoRestored.getMatrixDouble();
    QVERIFY(matrix_ptr);
    QVERIFY(*matrix_ptr == matrixDouble);
}

void TestClass::test_protocol_string()
{
    QString str = "Hello, world!";
    TCP::Protocol::Proto proto{&str};

    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out << proto;

    // Восстановление данных из QByteArray
    TCP::Protocol::Proto protoRestored;
    QDataStream in(&byteArray, QIODevice::ReadOnly);
    in >> protoRestored;

    QVERIFY(!protoRestored.getMatrixDouble());
    QVERIFY(!protoRestored.getMatrixInt());

    auto string_ptr = protoRestored.getString();
    QVERIFY(string_ptr);
    QVERIFY(*string_ptr == str);
}

void TestClass::compareMessageFromServer(TCP::Protocol::Proto proto)
{
    if (auto string_ptr = proto.getString())
    {
        auto const& msg = *string_ptr;
        QVERIFY(msg == QString("Helloy, client"));
    }
    else if (auto matrix_ptr = proto.getMatrixInt())
    {
        auto const& matrix_received = *matrix_ptr;
        QVERIFY(matrix_received == matrix);
    }
    else
    {
        QVERIFY(false);
    }
}

void TestClass::compareMessageFromClient(qintptr clientId, TCP::Protocol::Proto proto)
{
    if (auto string_ptr = proto.getString())
    {
        auto const& msg = *string_ptr;
        QVERIFY(msg == QString("Helloy, server"));
    }
    else if (auto matrix_ptr = proto.getMatrixInt())
    {
        auto const& matrix_received = *matrix_ptr;
        QVERIFY(matrix_received == matrix);
    }
    else
    {
        QVERIFY(false);
    }

    QString msgFromServer = "Helloy, client";
    srv1.sendMessage(clientId, &msgFromServer); // send string
    srv1.sendMessage(clientId, &matrix); // send matrix
}

void TestClass::compareMessageFromClientTimeout(qintptr, TCP::Protocol::Proto proto)
{
    if (auto string_ptr = proto.getString())
    {
        auto const& msg = *string_ptr;
        QVERIFY(msg == QString("Helloy, server"));
    }
    else if (auto matrix_ptr = proto.getMatrixInt())
    {
        auto const& matrix_received = *matrix_ptr;
        QVERIFY(matrix_received == matrix);
    }
    else
    {
        QVERIFY(false);
    }

    QTest::qWait(TCP::RESPONSE_TIMEOUT + 1000);
}

void TestClass::client_server_communication()
{
    connect(&srv1, &TCP::TCPServer::newMessage, this, &TestClass::compareMessageFromClient);
    connect(&cli1, &TCP::TCPClient::newMessage, this, &TestClass::compareMessageFromServer);

    QString msg = "Helloy, server";
    cli1.sendMessage(&msg);
    QTest::qWait(100); // wait server response

    cli1.sendMessage(&matrix);
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

    QString msg = "Helloy, server";
    cli2.sendMessage(&msg);
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

    QString msg = "Helloy, server";
    client.sendMessage(&msg); // сообщение к неподключённому серверу
    QVERIFY(cli2.isConnected() == false);

    QString msgToNobody = "Hi nobody";
    server.sendMessage(qintptr(), &msgToNobody); // сообщение в никуда
}

void TestClass::test_reconnection()
{
    // TODO: uncomment after investigate
    cli1.disconnect();
    QVERIFY(cli1.isConnected() == false);

    bool connected = cli1.connectToHost(PORT2);
    QVERIFY(connected == true);

    cli1.disconnect();
    QVERIFY(cli1.isConnected() == false);

    connected = cli1.connectToHost(PORT2);
    QVERIFY(connected == true);

    // TODO: uncomment during TRH-70
    // // message after reconnection
    // client_server_communication();
}

QTEST_GUILESS_MAIN(TestClass)
#include "test.moc"
