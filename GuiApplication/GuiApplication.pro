QT += quick location widgets webenginewidgets network

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
        main.cpp \
        objects/RightToolBar/righttoolbar.cpp \
        objects/TopToolBar/toptoolbar.cpp \
        objects/map_screen.cpp \
        objects/mapwidget.cpp \
        transhubwingow.cpp
//я люблю своих друзей




include(./../include/settings.pri)

RESOURCES += \
    qml.qrc

TRANSLATIONS += \
    GuiApplication_en_GB.ts
CONFIG += lrelease
CONFIG += embed_translations
CONFIG   += mobility



MOBILITY  += location
PKGCONFIG += openssl

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

HEADERS += \
    objects/RightToolBar/righttoolbar.h \
    objects/TopToolBar/toptoolbar.h \
    objects/map_screen.h \
    objects/mapwidget.h \
    transhubwingow.h
