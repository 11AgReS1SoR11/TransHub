TEMPLATE = app
TARGET = RoutingTest

QT += testlib core
LIBS += -L$$PWD/../../../build/Common/ComponentRouting/src -lComponentRouting

INCLUDEPATH += ../headers

# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects

SOURCES += test.cpp
