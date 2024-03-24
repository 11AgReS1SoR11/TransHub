TEMPLATE = app
TARGET = MatrixTest

QT += widgets testlib

SOURCES += test.cpp

INCLUDEPATH +=  ../src

LIBS += -L$$PWD/../../../build/Common/Matrix/src -lMatrix