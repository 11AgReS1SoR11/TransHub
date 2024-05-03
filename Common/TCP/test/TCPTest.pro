TEMPLATE = app
TARGET = TCPTest

QT += network testlib core
LIBS += -L$$PWD/../../../build/Common/TCP/src -lTCP
LIBS += -L$$PWD/../../../build/Common/CommonStructures -lCommonStructures

INCLUDEPATH += ../headers
INCLUDEPATH += ../../ComponentMatrix/src/
INCLUDEPATH += ../../CommonStructures/headers

# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects

SOURCES += test.cpp

