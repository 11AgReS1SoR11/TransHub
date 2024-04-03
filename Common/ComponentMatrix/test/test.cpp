#include <QtTest/QtTest>
#include "Matrix.hpp"


using namespace Mtx;

class TestClass : public QObject{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void test_case1();
    void test_case2();
    void test_case3();
    void test_case4();
    void test_case5();
    void test_case6();
    void test_case7();
    void cleanupTestCase();
};

void TestClass::initTestCase()
{
    qDebug() << "Called before everything else.";
}


void TestClass::test_case1(){

    QVector<QVector<int>> vec1 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    QVector<QVector<int>> vec2 = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    Matrix mat1(vec1);
    Matrix mat2(vec2);

    Matrix mat3(mat1 + mat2);

    Matrix mat_exp(QVector<QVector<int>> {
        {10,10,10},
        {10,10,10},
        {10,10,10}
    });


    QVERIFY(mat3 == mat_exp);
}

void TestClass::test_case2(){

    QVector<QVector<int>> vec1 = {
        {1, 2, 3},
        {4, 5, 6}
    };

    QVector<QVector<int>> vec2 = {
        {9, 7},
        {5, 4},
        {2, 1}
    };

    Matrix mat1(vec1);
    Matrix mat2(vec2);
    

    Matrix mat3(mat2 * mat1);

    Matrix mat_exp(QVector<QVector<int>> {
        {37, 53, 69}, 
        {21, 30, 39}, 
        {6, 9, 12}
    });
    QVERIFY(mat3 == mat_exp);

}

void TestClass::test_case3(){
    QVector<QVector<int>> vec1 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    QVector<QVector<int>> vec2 = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    Matrix mat1(vec1);
    Matrix mat2(vec2);

    Matrix mat3 = mat1.concatenation(mat2);

    Matrix mat_exp(QVector<QVector<int>> {
        {1, 2, 3, 9, 8, 7},
        {4, 5, 6, 6, 5, 4},
        {7, 8, 9, 3, 2, 1}
    });

    QVERIFY(mat3 == mat_exp);
}

void TestClass::test_case4(){

    QVector<QVector<int>> vec1 = {
        {1, 2, 3},
        {4, 5},
        {7, 8, 9}
    };


    QVERIFY_EXCEPTION_THROWN(Matrix mat1(vec1), QException);
}

void TestClass::test_case5(){
    QVector<QVector<int>> vec1 = {
        {1, 2, 3},
        {4, 5, 6}
    };

    QVector<QVector<int>> vec2 = {
        {4, 6, 3},
        {1, 2, 9}
    };

    Matrix mat1(vec1);
    Matrix mat2(vec2);


    QVERIFY_EXCEPTION_THROWN(mat1 * mat2, QException);
}
void TestClass::test_case6(){
    QVector<QVector<int>> vec1 = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    Matrix mat1(vec1);

    QVERIFY(mat1.determinant() == 0);

    QVector<QVector<int>> vec2 = {
        {9, 7},
        {5, 4},
        {2, 1}
    };

    Matrix mat2(vec2);

    QVERIFY_EXCEPTION_THROWN(mat2.determinant(), QException);
}

void TestClass::test_case7(){
    QVector<QVector<int>> vec1 = {
        {1, 2, 3},
        {4, 5, 6}
    };

    QVector<QVector<int>> vec2 = {
        {9, 7},
        {5, 4},
        {2, 1}
    };

    Matrix mat1(vec1);
    Matrix mat2(vec2);

    QVERIFY_EXCEPTION_THROWN(mat1 + mat2, QException);
}
void TestClass::cleanupTestCase(){
    qDebug() <<  "Called after all tests.";
}


QTEST_GUILESS_MAIN(TestClass)
#include "test.moc"
