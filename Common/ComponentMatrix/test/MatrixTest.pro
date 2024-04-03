TEMPLATE = app
TARGET = MatrixTest

QT += testlib

SOURCES += test.cpp

INCLUDEPATH +=  ../src

LIBS += -L$$PWD/../../../build/Common/ComponentMatrix/src -lComponentMatrix