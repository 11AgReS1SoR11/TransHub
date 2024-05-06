#include <QtTest/QtTest>
#include "TCPClient.hpp"
#include "ServiceManager.hpp"
#include "TCPProto.hpp"

class TestClass : public QObject
{
    Q_OBJECT

private:
    TCP::TCPClient client1;
    TCP::TCPClient client2;
    TCP::TCPClient client3;
    ServiceManager manager;

    bool routesRecieved{false};
    bool lengthRecieved{false};

    void compareMessageFromServer(TCP::Protocol::Proto& proto);
    void clientSendToServer(TCP::TCPClient& client);
    void reset();

private slots:
    void test_multipleCalling();
    void test_sunny();
    void test_multipleCallingWithDisconect();
};

void TestClass::reset()
{
    routesRecieved = false;
    lengthRecieved = false; 
}

void TestClass::compareMessageFromServer(TCP::Protocol::Proto& proto)
{
    if (auto string_ptr = proto.get<QString>())
    {
        auto const& msg = *string_ptr;
        double answer = msg.toDouble();
        // 25'000 <= answer <= 35'000
        QVERIFY(answer >= 25000.0);
        QVERIFY(answer <= 35000.0);

        lengthRecieved = true;
    }
    else if (auto matrix_ptr = proto.get<Mtx::Matrix<double>>())
    {
        auto const& matrix_received = *matrix_ptr;

        Mtx::Matrix<double> expected_result = {{1, 2, 2, 7723},
                                               {2, 2, 2, 6706},
                                               {3, 1, 1, 6176},
                                               {4, 1, 3, 6329}};

        // Solution can change and expected_result it is not only one right solution
        QVERIFY(matrix_received.rows() == expected_result.rows());
        QVERIFY(matrix_received.columns() == expected_result.columns());

        routesRecieved = true;
    }
    else
    {
        QVERIFY(false);
    }
}

void TestClass::clientSendToServer(TCP::TCPClient& client)
{
    QVector<int> StoragesCapacity = {3, 3}; // storages   sum(a) = 6
    QVector<int> ClientsCapacity = {1, 2, 1}; // clients   sum(b) = 4
    QVector<int> CouriersCapacity = {1, 1, 1, 1, 1}; // couriers   sum(d) = 5 => cannot draw conclusions about the optimality of the naive solution

    const Common::Coordinate Lesnaya = {.lat = 59.98480089390978, .lon = 30.344101989163246};
    const Common::Coordinate Pionerka = {.lat = 60.00461610457039, .lon = 30.29602857606911};
    const Common::Coordinate Udelka = {.lat = 60.01873067647047, .lon = 30.317798421597637};
    const Common::Coordinate Politehnicheskaya = {.lat = 60.01010225177014, .lon = 30.373730259933833};
    const Common::Coordinate ChornayaRechka = {.lat = 59.985582403512005, .lon = 30.301090499706547};

    Common::Coordinates_t CouriersCoordinates;
    CouriersCoordinates.push_back({(Lesnaya.lat + Pionerka.lat) / 2, (Lesnaya.lon + Pionerka.lon) / 2});
    CouriersCoordinates.push_back({(Lesnaya.lat + Udelka.lat) / 2, (Lesnaya.lon + Udelka.lon) / 2});
    CouriersCoordinates.push_back({(Lesnaya.lat + Politehnicheskaya.lat) / 2, (Lesnaya.lon + Politehnicheskaya.lon) / 2});
    CouriersCoordinates.push_back({(Lesnaya.lat + ChornayaRechka.lat) / 2, (Lesnaya.lon + ChornayaRechka.lon) / 2});
    CouriersCoordinates.push_back({(Udelka.lat + Politehnicheskaya.lat) / 2, (Udelka.lon + Politehnicheskaya.lon) / 2});

    Common::Coordinates_t StoragesCoordinates = { Lesnaya, Pionerka };
    Common::Coordinates_t ClientsCoordinates = { ChornayaRechka, Udelka, Politehnicheskaya };

    // TODO: сделать нормально TRH-84
    QTest::qWait(10);
    QString msg1 = "Clients";
    client.sendMessage(&msg1);
    QTest::qWait(10);
    client.sendMessage(&ClientsCoordinates);

    QTest::qWait(10);
    QString msg2 = "Storages";
    client.sendMessage(&msg2);
    QTest::qWait(10);
    client.sendMessage(&StoragesCoordinates);

    QTest::qWait(10);
    QString msg3 = "Couriers";
    client.sendMessage(&msg3);
    QTest::qWait(10);
    client.sendMessage(&CouriersCoordinates);

    QTest::qWait(10);
    QString msg4 = "Clients capacity";
    client.sendMessage(&msg4);
    QTest::qWait(10);
    client.sendMessage(&ClientsCapacity);

    QTest::qWait(10);
    QString msg5 = "Storages capacity";
    client.sendMessage(&msg5);
    QTest::qWait(10);
    client.sendMessage(&StoragesCapacity);

    QTest::qWait(10);
    QString msg6 = "Couriers capacity";
    client.sendMessage(&msg6);
    QTest::qWait(10);
    client.sendMessage(&CouriersCapacity);

    bool massagesRecieved = QTest::qWaitFor([&]() {
        return routesRecieved && lengthRecieved;
    }, 20000); // wait 2 messages or 20 seconds

    QVERIFY(massagesRecieved);
    reset();
}

void TestClass::test_sunny()
{
    connect(&client1, &TCP::TCPClient::newMessage, this, &TestClass::compareMessageFromServer);
    bool connected = client1.connectToHost(PORT);
    QVERIFY(connected == true);

    clientSendToServer(client1);

    client1.disconnect();
    QVERIFY(client1.isConnected() == false);
}

void TestClass::test_multipleCalling()
{
    connect(&client2, &TCP::TCPClient::newMessage, this, &TestClass::compareMessageFromServer);
    bool connected = client2.connectToHost(PORT);
    QVERIFY(connected == true);

    for (int i = 0; i < 3; ++i)
    {
        clientSendToServer(client2);
    }

    client2.disconnect();
    QVERIFY(client2.isConnected() == false);
}

void TestClass::test_multipleCallingWithDisconect()
{
    // TODO: uncomment during TRH-70
    // connect(&client3, &TCP::TCPClient::newMessage, this, &TestClass::compareMessageFromServer);

    // for (int i = 0; i < 3; ++i)
    // {
    //     bool connected = client3.connectToHost(PORT);
    //     QVERIFY(connected == true);
    //     QTest::qWait(1000);
    //     clientSendToServer(client3);
    //     QTest::qWait(1000);
    //     client3.disconnect();
    //     QVERIFY(client3.isConnected() == false);
    //     QTest::qWait(1000);
    // }
}

QTEST_GUILESS_MAIN(TestClass)
#include "test.moc"
