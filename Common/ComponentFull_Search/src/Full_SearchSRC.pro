TEMPLATE = lib
TARGET = $$qtLibraryTarget(ComponentFull_Search)

SOURCES += Object/Full_Search.cpp \
           ComponentFull_Search.cpp

CONFIG += plugin

HEADERS += Object/Full_Search.hpp \
           ComponentFull_Search.hpp \
           ComponentFull_Search_global.h

INCLUDEPATH += ../../Matrix/src
INCLUDEPATH += ../../Application/
INCLUDEPATH += ../../3rdparty/
INCLUDEPATH += ../../IObject/ISystemGuiCore/
INCLUDEPATH += ../../

# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects


DISTFILES += \
    ComponentSystemGuiCoreNg.json
