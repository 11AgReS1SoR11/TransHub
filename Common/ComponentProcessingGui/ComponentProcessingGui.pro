include (../IObject/ISystemGuiCore/ISystemGuiCore.pri)

QT += core gui widgets
TARGET = $$qtLibraryTarget(ComponentsProcessingGui)
TEMPLATE = lib
CONFIG += c++14
CONFIG += plugin

INCLUDEPATH += ../Application/
INCLUDEPATH += ../3rdparty/
INCLUDEPATH += ../IObject/ISystemGuiCore/
INCLUDEPATH += ../


# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects

DEFINES += COMPONENTPROCESSINGGUI_LIBRARY
TRANSLATIONS += ComponentProcessingGui.ts

SOURCES += \
    ComponentProcessingGui.cpp      \
    Object/ProcessingGui.cpp        \
    Object/Settings.cpp \
    Object/MapWidget.cpp \
    Object/CommandPanel.cpp \
    Object/ResourceManager.cpp

HEADERS += \
    ComponentProcessingGui.h        \
    ComponentProcessingGui_global.h \
    Object/ProcessingGui.h          \
    Object/Settings.h \
    Object/MapWidget.h \
    Object/CommandPanel.h \
    Object/ResourceManager.h

FORMS += \
    Object/RequestWidget.ui \
    Object/MapWidget.ui \
    Object/CommandPanel.ui

DISTFILES += \
    Changelist.txt \
    ComponentProcessingGui.json \
    ComponentProcessingGui.ts

RESOURCES += \
    resources.qrc
