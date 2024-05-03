TEMPLATE = app
TARGET = ServiceApplication

# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects

LIBS += -L$$PWD/../../build/Common/TCP/src -lTCP
LIBS += -L$$PWD/../../build/Common/ComponentFull_Search/src -lComponentFull_Search
LIBS += -L$$PWD/../../build/Common/ComponentMatrix/src -lComponentMatrix
LIBS += -L$$PWD/../../build/Common/ComponentRouting/src -lComponentRouting
LIBS += -L$$PWD/../../build/Common/CommonStructures -lCommonStructures

LIBS += -L$$PWD/../../build/Common/ComponentFull_Search/src -lComponentFull_Search
LIBS += -L$$PWD/../../build/Common/ComponentsCore5alpha -lComponentsCore5alpha

QT += core network

SOURCES += main.cpp ServiceManager.cpp

HEADERS += ServiceManager.hpp

INCLUDEPATH +=../../Common/TCP/headers
INCLUDEPATH +=../../Common/ComponentRouting/headers
INCLUDEPATH +=../../Common/CommonStructures/headers
INCLUDEPATH +=../../Common/ComponentFull_Search/src/Object
INCLUDEPATH +=../../Common/ComponentMatrix/src
