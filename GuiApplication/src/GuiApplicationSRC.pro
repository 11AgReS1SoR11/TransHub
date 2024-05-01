TARGET = GuiApplication
TEMPLATE = app

QT += core gui network widgets location quick webenginewidgets webengine

LIBS += -L$$PWD/../../build/Common/ComponentsCore5alpha -lComponentsCore5alpha
LIBS += -L$$PWD/../../build/Common/ComponentSystemGuiCoreNg -lComponentSystemGuiCoreNg

SOURCES += \
        InformWidget.cpp \
        LoadDialog.cpp \
        RunErrorDialog.cpp \
        SplashScreen.cpp \
        UnloadDialog.cpp \
        main.cpp \
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
    QtLanguage_ru.ts \


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
    transhubwingow.h \
    ../../Common/ColorConsole/ColorConsoleAppender.h

FORMS += \
    InformWidget.ui \
    LoadDialog.ui \
    RunErrorDialog.ui \
    UnloadDialog.ui


COMMON_PATH = $$PWD/../../build/Common
COMPONENT_FULL_SEARCH = $$COMMON_PATH/ComponentFull_Search/src/libComponentFull_Search.so
COMPONENT_PROCESSING_GUI = $$COMMON_PATH/ComponentProcessingGui/libComponentProcessingGui.so
COMPONENT_SYSTE_GUI_CORE = $$COMMON_PATH/ComponentSystemGuiCoreNg/libComponentSystemGuiCoreNg.so


GUIAPPLICATION_PATH_DICTIONARIES = $$PWD/../../Common/ComponentProcessingGui/dictionaries


first.depends = checkdirectory copyfile
checkdirectory.commands = test -d $$OUT_PWD/components || mkdir -p $$OUT_PWD/components
copyfile.commands = cp $$COMPONENT_FULL_SEARCH $$OUT_PWD/components/ \
                    && cp $$COMPONENT_PROCESSING_GUI $$OUT_PWD/components/ \
                    && cp $$COMPONENT_SYSTE_GUI_CORE $$OUT_PWD/components/ \
                    && cp $$PWD/../../Common/ComponentProcessingGui/ComponentProcessingGui.qm $$OUT_PWD/ \
                    && cp $$PWD/../../Common/ComponentSystemGuiCoreNg/ComponentSystemGuiCoreNg.qm $$OUT_PWD/ \
                    && cp $$PWD/../../GuiApplication/src/QtLanguage_ru.qm $$OUT_PWD/ \

export(first.depends)
export(checkdirectory.commands)
export(copyfile.commands)

QMAKE_EXTRA_TARGETS += first checkdirectory copyfile


