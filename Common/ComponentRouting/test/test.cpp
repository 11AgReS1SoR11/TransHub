#include <QtTest/QtTest>

#include "Routing.hpp"

static constexpr Routing::Coordinate Moscow = {.lat = 55.7522, .lon = 37.6156};
static constexpr Routing::Coordinate SPB = {.lat = 59.9386, .lon = 30.3141};
static constexpr Routing::Coordinate Lesnaya = {.lat = 59.98480089390978, .lon = 30.344101989163246};
static constexpr Routing::Coordinate Pionerka = {.lat = 60.00461610457039, .lon = 30.29602857606911};
static constexpr Routing::Coordinate Udelka = {.lat = 60.01873067647047, .lon = 30.317798421597637};
static constexpr Routing::Coordinate Politehnicheskaya = {.lat = 60.01010225177014, .lon = 30.373730259933833};

// Moscow - SPB = 710km
// Lesnaya - Pionerka = 5.3km
// Lesnaya - Udelka = 5.2km
// Lesnaya - Politehnicheskaya = 3.9km
// Pionerka - Politehnicheskaya = 5.8km
// Pionerka - Udelka = 4.1km
// Udelka - Politehnicheskaya = 4.6km

// Moscow - SPB = 710km
// SPB - Moscow = 720km

class TestClass : public QObject
{
    Q_OBJECT

private slots:
    void test_case_SPB();
    void test_case_RU();
};

void TestClass::test_case_SPB()
{
    constexpr double epsilon = 300; // 300 metrov

    QVERIFY(abs(Routing::GetRoute(Lesnaya, Pionerka) - 5300) < epsilon);
    QVERIFY(abs(Routing::GetRoute(Lesnaya, Udelka) - 5200) < epsilon);
    QVERIFY(abs(Routing::GetRoute(Lesnaya, Politehnicheskaya) - 3900) < epsilon);
    QVERIFY(abs(Routing::GetRoute(Pionerka, Politehnicheskaya) - 5800) < epsilon);
    QVERIFY(abs(Routing::GetRoute(Pionerka, Udelka) - 4100) < epsilon);
    QVERIFY(abs(Routing::GetRoute(Udelka, Politehnicheskaya) - 4600) < epsilon);
}

void TestClass::test_case_RU()
{
    constexpr double epsilon = 10000; // 10 km

    QVERIFY(abs(Routing::GetRoute(Moscow, SPB) - 710000) < epsilon);
    QVERIFY(abs(Routing::GetRoute(SPB, Moscow) - 720000) < epsilon);
}

QTEST_GUILESS_MAIN(TestClass)
#include "test.moc"
