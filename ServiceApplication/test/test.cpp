#include <QtTest/QtTest>

class TestClass : public QObject
{
    Q_OBJECT
private slots:
    void test_case1();
    void test_case2();
};

void TestClass::test_case1()
{
    QVERIFY(2 == 2);
}

void TestClass::test_case2()
{
    QVERIFY(2 == 2);
}

QTEST_GUILESS_MAIN(TestClass)
#include "test.moc"
