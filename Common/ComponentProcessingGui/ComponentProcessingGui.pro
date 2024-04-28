include (../IObject/ISystemGuiCore/ISystemGuiCore.pri)

QT += core gui network widgets location quick webengine webenginewidgets
TARGET = $$qtLibraryTarget(ComponentProcessingGui)
TEMPLATE = lib
CONFIG += c++14
CONFIG += plugin

LIBS += -L$$PWD/../../build/Common/ComponentSystemGuiCoreNg -lComponentSystemGuiCoreNg

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
#DESTDIR = ../../GuiApplication/src/components

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
    Object/StyleHelper.cpp \
    Object/planningmanager.cpp

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
    Object/StyleHelper.h \
    Object/planningmanager.h

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

