#include <QtTest/QtTest>
#include "../src/Full_Search.hpp"

class TestClass : public QObject
{
    Q_OBJECT
private slots:
    void test_case1();
    void test_case2();
};

void TestClass::test_case1()
{
    Algo::vector<int> a = {3, 3}; // storages   sum(a) = 6
    Algo::vector<int> b = {1, 2, 1}; // clients   sum(b) = 4
    Algo::vector<int> d = {1, 1, 1, 1, 1}; // couriers   sum(d) = 5 => cannot draw conclusions about the optimality of the naive solution

    // matrix storages-clients
    Algo::matrix<int> C = {{2, 4, 5},
                           {5, 5, 4}};

    // matrices couriers-storages (excluding the 5th storage)
    Algo::matrix<int> D = {{6, 3},
                           {7, 3},
                           {3, 1},
                           {5, 1},
                           {4, 1}};

    Algo::matrix<int> expected_result = {{1, 2, 2, 8},
                                         {3, 1, 1, 5},
                                         {4, 2, 2, 6},
                                         {5, 2, 3, 5}};

    auto [routes, length] = Algo::Full_search(a, b, d, C, D);
    QVERIFY(true == Algo::CompareMatrix(routes, expected_result));
    QVERIFY(length == 24);
}

void TestClass::test_case2()
{
    // TEST IS TOO LONG TO WAIT (5 min)
    // // sum(a) = sum(b) = sum(d) = 12 CORRECT!
    // vector<int> a = {1, 4, 4, 5, 3}; // storages
    // vector<int> b = {3, 2, 4, 2}; // clients
    // vector<int> d = {1, 1, 1, 1, 1, 1, 1}; // couriers
    // // matrix storages-clients
    // matrix<int> C = {{5, 13, 10, 2},
    //                  {3, 9, 6, 4},
    //                  {3, 5, 5, 8},
    //                  {6, 6, 3, 7},
    //                  {11, 7, 4, 6}};

    // // matrices couriers-storages (excluding the 5th storage)
    // matrix<int> D = {{8, 4, 4, 7, 12},
    //                 {13, 9, 5, 6, 7},
    //                 {4, 4, 4, 3, 4},
    //                 {5, 5, 5, 2, 3},
    //                 {5, 7, 7, 6, 5},
    //                 {6, 8, 8, 5, 2},
    //                 {10, 8, 8, 5, 4}};

    // auto road_and_length = Algo::Full_search(a, b, d, C, D);
    // Algo::print_matrix(road_and_length.routes);
    // QVERIFY(road_and_length.length == 47);
}

QTEST_GUILESS_MAIN(TestClass)
#include "test.moc"
