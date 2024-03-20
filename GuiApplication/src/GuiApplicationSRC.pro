TARGET = GuiApplication
TEMPLATE = app

QT += core gui network widgets location quick
LIBS += -L$$PWD/../../build/Common/ComponentsCore5alpha -lComponentsCore5alpha

SOURCES += \
        InformWidget.cpp \
        LoadDialog.cpp \
        RunErrorDialog.cpp \
        SplashScreen.cpp \
        UnloadDialog.cpp \
        main.cpp \
        objects/RightToolBar/righttoolbar.cpp \
        objects/TopToolBar/toptoolbar.cpp \
        objects/map_screen.cpp \
        objects/mapwidget.cpp \
        transhubwingow.cpp \
        ../../Common/ColorConsole/ColorConsoleAppender.cpp

include(../../Common/IObject/ISystemGuiCore/ISystemGuiCore.pri)
include(../../Common/Application/application.pri)
include(../../Common/3rdparty/log4qt/log4qt.pri)

CONFIG += c++14

OBJECTS_DIR = objects
MOC_DIR = objects
UI_DIR = objects
RCC_DIR = objects

CONFIG(debug, debug|release) {
CONFIG -= debug release
CONFIG += debug
DEFINES += DEBUG
}


CONFIG(release, debug|release) {
CONFIG -= debug release
CONFIG += release
DEFINES += RELEASE
}

INCLUDEPATH +=../../Common/
INCLUDEPATH += ../../Common/Application/
INCLUDEPATH += ../../Common/3rdparty/
INCLUDEPATH += ../../Common/IObject/ISystemGuiCore/
INCLUDEPATH += ../../build/Common/ComponentsCore5alpha/

RESOURCES += \
    qml.qrc

TRANSLATIONS += \
    GuiApplication_en_GB.ts
CONFIG += lrelease
CONFIG += embed_translations
CONFIG   += mobility

MOBILITY  += location
PKGCONFIG += openssl

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    GuiVersion.h \
    InformWidget.h \
    LoadDialog.h \
    RunErrorDialog.h \
    SplashScreen.h \
    UnloadDialog.h \
    functions.h \
    objects/RightToolBar/righttoolbar.h \
    objects/TopToolBar/toptoolbar.h \
    objects/map_screen.h \
    objects/mapwidget.h \
    transhubwingow.h \
    ../../Common/ColorConsole/ColorConsoleAppender.h \

FORMS += \
    InformWidget.ui \
    LoadDialog.ui \
    RunErrorDialog.ui \
    UnloadDialog.ui
