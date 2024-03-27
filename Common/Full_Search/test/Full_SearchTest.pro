TEMPLATE = app
TARGET = Full_SearchTest

QT += testlib

SOURCES += test.cpp

INCLUDEPATH +=  ../src
INCLUDEPATH += ../../Matrix/src

LIBS += -L$$PWD/../../../build/Common/Full_Search/src -lFull_Search