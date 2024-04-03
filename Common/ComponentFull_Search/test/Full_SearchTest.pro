TEMPLATE = app
TARGET = Full_SearchTest

QT += testlib

SOURCES += test.cpp

INCLUDEPATH +=  ../src/Object
INCLUDEPATH += ../../ComponentMatrix/src

LIBS += -L$$PWD/../../../build/Common/ComponentFull_Search/src -lComponentFull_Search
LIBS += -L$$PWD/../../../build/Common/ComponentsCore5alpha -lComponentsCore5alpha