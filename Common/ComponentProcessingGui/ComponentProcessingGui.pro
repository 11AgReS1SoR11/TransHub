include (../IObject/ISystemGuiCore/ISystemGuiCore.pri)

QT += core gui network widgets location quick webengine webenginewidgets webchannel
TARGET = $$qtLibraryTarget(ComponentProcessingGui)
TEMPLATE = lib
CONFIG += c++17
CONFIG += plugin

LIBS += -L$$PWD/../../build/Common/ComponentSystemGuiCoreNg -lComponentSystemGuiCoreNg
LIBS += -L$$PWD/../../build/Common/TCP/src -lTCP
LIBS += -L$$PWD/../../build/Common/ComponentMatrix/src -lComponentMatrix
LIBS += -L$$PWD/../../build/Common/CommonStructures -lCommonStructures

MOBILITY  += location
PKGCONFIG += openssl

QT += testlib

INCLUDEPATH += ../Application/
INCLUDEPATH += ../3rdparty/
INCLUDEPATH += ../IObject/ISystemGuiCore/
INCLUDEPATH += ../
INCLUDEPATH +=../TCP/headers
INCLUDEPATH +=../ComponentMatrix/src
INCLUDEPATH +=../CommonStructures/headers

# Build dirs
OBJECTS_DIR = objects
MOC_DIR     = objects
UI_DIR      = objects
RCC_DIR     = objects
#DESTDIR = ../../GuiApplication/src/components

DEFINES += COMPONENTPROCESSINGGUI_LIBRARY
TRANSLATIONS += ComponentProcessingGui.ts

SOURCES += \
    ComponentProcessingGui.cpp      \
    Object/MapWidget.cpp \
    Object/ProcessingGui.cpp        \
    Object/ProcessingGuiTabWindowsManager.cpp \
    Object/Settings.cpp \
    Object/PlanningWidget.cpp \
    Object/CommandPanel.cpp \
    Object/ResourceManager.cpp \
    Object/StatusBarMapWidget.cpp \
    Object/StatusBarMapWidgetToolTip.cpp \
    Object/StatusBarPlanningWidget.cpp \
    Object/StatusBarPlanningWidgetToolTip.cpp \
    Object/StyleHelper.cpp \
    Object/planningmanager.cpp

HEADERS += \
    ComponentProcessingGui.h        \
    ComponentProcessingGui_global.h \
    Object/MapWidget.h \
    Object/PointCoordinates.h \
    Object/ProcessingGui.h          \
    Object/ProcessingGuiTabWindowsManager.h \
    Object/Settings.h \
    Object/PlanningWidget.h \
    Object/CommandPanel.h \
    Object/ResourceManager.h \
    Object/StatusBarMapWidget.h \
    Object/StatusBarMapWidgetToolTip.h \
    Object/StatusBarPlanningWidget.h \
    Object/StatusBarPlanningWidgetToolTip.h \
    Object/StyleHelper.h \
    Object/planningmanager.h

FORMS += \
    Object/MapWidget.ui \
    Object/RequestWidget.ui \
    Object/PlanningWidget.ui \
    Object/CommandPanel.ui \
    Object/StatusBarMapWidgetToolTip.ui \
    Object/StatusBarPlanningWidgetToolTip.ui

DISTFILES += \
    Changelist.txt \
    ComponentProcessingGui.json \
    ComponentProcessingGui.ts \
    osm.html \
    qwebchannel.js \
    qwebchannel.js \
    storage2.png \
    user.png

RESOURCES += \
    resources.qrc

#basePath = $$PWD/../..
#sourceLib = $$OUT_PWD/libComponentFull_Search.so
#targetDir = $$basePath/build/GuiApplication/src/components

#first.depends = checkdirectory copyfile
#checkdirectory.commands = test -d $$targetDir || mkdir -p $$targetDir
#copyfile.commands = cp $$sourceLib $$targetDir

#export(first.depends)
#export(checkdirectory.commands)
#export(copyfile.commands)

#QMAKE_EXTRA_TARGETS += first checkdirectory copyfile

