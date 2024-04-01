include (../IObject/ISystemGuiCore/ISystemGuiCore.pri)

QT += core gui network widgets location quick webengine webenginewidgets
TARGET = $$qtLibraryTarget(ComponentsProcessingGui)
TEMPLATE = lib
CONFIG += c++14
CONFIG += plugin

MOBILITY  += location
PKGCONFIG += openssl


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
    Object/GuiWidgetsManager.cpp \
    Object/MapWidget.cpp \
    Object/ProcessingGui.cpp        \
    Object/Settings.cpp \
    Object/PlanningWidget.cpp \
    Object/CommandPanel.cpp \
    Object/ResourceManager.cpp \
    Object/StyleHelper.cpp

HEADERS += \
    ComponentProcessingGui.h        \
    ComponentProcessingGui_global.h \
    Object/GuiWidgetsManager.h \
    Object/MapWidget.h \
    Object/ProcessingGui.h          \
    Object/Settings.h \
    Object/PlanningWidget.h \
    Object/CommandPanel.h \
    Object/ResourceManager.h \
    Object/StyleHelper.h

FORMS += \
    Object/MapWidget.ui \
    Object/RequestWidget.ui \
    Object/PlanningWidget.ui \
    Object/CommandPanel.ui

DISTFILES += \
    Changelist.txt \
    ComponentProcessingGui.json \
    ComponentProcessingGui.ts \
    osm.html

RESOURCES += \
    resources.qrc
