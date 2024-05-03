TEMPLATE = app
TARGET = RoutingTest

QT += testlib core
LIBS += -L$$PWD/../../../build/Common/ComponentRouting/src -lComponentRouting
LIBS += -L$$PWD/../../../build/Common/CommonStructures -lCommonStructures

INCLUDEPATH += ../headers
INCLUDEPATH += ../../CommonStructures/headers

# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects

SOURCES += test.cpp
