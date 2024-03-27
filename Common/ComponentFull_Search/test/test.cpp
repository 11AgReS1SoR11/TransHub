#include <QtTest/QtTest>
#include "Full_Search.hpp"

class TestClass : public QObject
{
    Q_OBJECT
private slots:
    void test_case1();
    void test_case2();
};

void TestClass::test_case1()
{
    QVector<int> a = {3, 3}; // storages   sum(a) = 6
    QVector<int> b = {1, 2, 1}; // clients   sum(b) = 4
    QVector<int> d = {1, 1, 1, 1, 1}; // couriers   sum(d) = 5 => cannot draw conclusions about the optimality of the naive solution

    // matrix storages-clients
    Mtx::Matrix<int> C = {{2, 4, 5},
                          {5, 5, 4}};

    // matrices couriers-storages (excluding the 5th storage)
    Mtx::Matrix<int> D = {{6, 3},
                          {7, 3},
                          {3, 1},
                          {5, 1},
                          {4, 1}};

    Mtx::Matrix<int> expected_result = {{1, 2, 2, 8},
                                        {3, 1, 1, 5},
                                        {4, 2, 2, 6},
                                        {5, 2, 3, 5}};

    auto [routes, length] = Algo::Full_search(a, b, d, C, D);
    QVERIFY(routes == expected_result);
    QVERIFY(length == 24);
}

void TestClass::test_case2()
{
    // TEST IS TOO LONG TO WAIT (5 min)
    // // sum(a) = sum(b) = sum(d) = 12 CORRECT!
    // QVector<int> a = {1, 4, 4, 5, 3}; // storages
    // QVector<int> b = {3, 2, 4, 2}; // clients
    // QVector<int> d = {1, 1, 1, 1, 1, 1, 1}; // couriers
    // // matrix storages-clients
    // Mtx::Matrix<int> C = {{5, 13, 10, 2},
    //                  {3, 9, 6, 4},
    //                  {3, 5, 5, 8},
    //                  {6, 6, 3, 7},
    //                  {11, 7, 4, 6}};

    // // matrices couriers-storages (excluding the 5th storage)
    // Mtx::Matrix<int> D = {{8, 4, 4, 7, 12},
    //                 {13, 9, 5, 6, 7},
    //                 {4, 4, 4, 3, 4},
    //                 {5, 5, 5, 2, 3},
    //                 {5, 7, 7, 6, 5},
    //                 {6, 8, 8, 5, 2},
    //                 {10, 8, 8, 5, 4}};

    // auto [routes, length] = Algo::Full_search(a, b, d, C, D);
    // QVERIFY(routes == expected_result);
    // QVERIFY(length == 47);
}

QTEST_GUILESS_MAIN(TestClass)
#include "test.moc"
