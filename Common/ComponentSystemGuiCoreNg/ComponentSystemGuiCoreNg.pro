include (../3rdparty/log4qt/log4qt.pri)
include (../IObject/ISystemGuiCore/ISystemGuiCore.pri)
include(../Application/application.pri)

#system ("mkdir -p $$OUT_PWD/../../../build/configs/")
#system ("cp -f $$PWD/*.xml $$OUT_PWD/../../../build/configs/")

## в отличе от system выполняется на этапе компиляции
#CONFIG += file_copies
#COPIES += cfgs
#cfgs.files = $$files($$PWD/*.xml)
#cfgs.path = $$OUT_PWD/../../../build/configs

QT += core widgets gui printsupport
TEMPLATE = lib
CONFIG += plugin

TARGET = $$qtLibraryTarget(ComponentSystemGuiCoreNg)

#win32: QT += multimedia
#unix: LIBS += -lasound

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

DEFINES += COMPONENTSYSTEMGUICORENG_LIBRARY
TRANSLATIONS += ComponentSystemGuiCoreNg.ts

SOURCES += \
    ComponentSystemGuiCoreNg.cpp \
    Object/MainWindow.cpp \
    Object/MdiArea/CustomMdiArea.cpp \
    Object/MdiArea/CustomMdiSubWindow.cpp \
    Object/SettingsPanel/SettingsGuiCoreWidget.cpp \
    Object/StatusBarInfo/StatusBarMapWidget.cpp \
    Object/StatusBarInfo/StatusBarMapWidgetToolTip.cpp \
    Object/StatusBarInfo/StatusBarPlanningWidget.cpp \
    Object/StatusBarInfo/StatusBarPlanningWidgetToolTip.cpp \
    Object/SystemGuiCoreSettings.cpp \
    Object/SystemIconManager.cpp \
    Object/WindowCreator/AboutComponentsWidget.cpp \
    Object/WindowCreator/AboutSystemDialog.cpp \
    Object/WindowCreator/SystemWindowsCreator.cpp \
    Object/BasePanel/BasePanelWidget.cpp \
    Object/BasePanel/BasePanelDockWidget.cpp \
    Object/Menu/SystemGuiCoreMenu.cpp \
    Object/ToolBar/SystemGuiCoreToolBar.cpp \
    Object/StatusBarInfo/StatusBarInfoWidget.cpp \
    Object/StatusBarInfo/StatusBarInfoWidgetData.cpp \
    Object/StatusBarInfo/StatusBarInfoWidgetPopupWindow.cpp \
    Object/StatusBarInfo/StatusBarInfoWidgetToolTip.cpp \
    Object/StatusBarInfo/SystemGuiCorePopUpElement.cpp \
    Object/StatusBarVolume/StatusBarVolume.cpp \
    Object/StatusBarVolume/StatusBarVolumeToolTip.cpp \
    Object/MainWindowConsole/MainWindowConsole.cpp \
    Object/StatusBarInfo/SystemWordWraper.cpp \
    Object/SettingsPanel/SettingsPanelContainer.cpp \
    Object/SettingsPanel/SettingsPanel.cpp \
    Object/SettingsPanel/SettingsBasePanel.cpp \
    Object/SettingsPanel/SettingsToolBar.cpp \
    Object/Delegates/TableBoolDelegate.cpp \
    Object/Delegates/TableColorDelegate.cpp \
    Object/Delegates/TableConvertDateTimeDelegate.cpp \
    Object/Delegates/TableConvertDateDelegate.cpp \
    Object/SettingsPanel/SettingsAppLoader.cpp \
    Object/MdiArea/MdiSubWindowFilter.cpp \
    Object/MdiArea/MdiAreaFilter.cpp \
    Object/StatusBar/StatusBarFilter.cpp \
    Object/HelpDesigner/HelpDesigner.cpp \
    Object/HelpDesigner/HelpSection.cpp \
    Object/HelpDesigner/HelpWindow.cpp \
    Object/HelpDesigner/HelpTextEdit.cpp \
    Object/HelpDesigner/HelpSearchWidget.cpp \
    Object/SettingsPanel/SettingsToolBarProps.cpp \
    Object/Delegates/GeoCoordinateDelegate.cpp \
    Object/StatusBarInfo/MouseEventFilter.cpp \
    Object/ExternalMainWindow.cpp \
    Object/SystemGuiCoreNg.cpp \
    Object/ExternalDialog.cpp \

HEADERS += \
    ComponentSystemGuiCoreNg.h \
    ComponentSystemGuiCoreNg_global.h \
    Object/GuiCoreTagDefines.h \
    Object/MainWindow.h \
    Object/MdiArea/CustomMdiArea.h \
    Object/MdiArea/CustomMdiSubWindow.h \
    Object/SettingsPanel/SettingsGuiCoreWidget.h \
    Object/StatusBarInfo/StatusBarMapWidget.h \
    Object/StatusBarInfo/StatusBarMapWidgetToolTip.h \
    Object/StatusBarInfo/StatusBarPlanningWidget.h \
    Object/StatusBarInfo/StatusBarPlanningWidgetToolTip.h \
    Object/SystemGuiCoreSettings.h \
    Object/SystemIconManager.h \
    Object/MainWindowConnector.h \
    Object/WindowCreator/AboutComponentsWidget.h \
    Object/WindowCreator/AboutSystemDialog.h \
    Object/WindowCreator/SystemWindowsCreator.h \
    Object/MainWindowDefines.h \
    Object/BasePanel/BasePanelWidget.h \
    Object/BasePanel/BasePanelDockWidget.h \
    Object/Menu/SystemGuiCoreMenu.h \
    Object/ToolBar/SystemGuiCoreToolBar.h \
    Object/StatusBarInfo/StatusBarInfoWidget.h \
    Object/StatusBarInfo/StatusBarInfoWidgetData.h \
    Object/StatusBarInfo/StatusBarInfoWidgetPopupWindow.h \
    Object/StatusBarInfo/StatusBarInfoWidgetToolTip.h \
    Object/StatusBarInfo/SystemGuiCorePopUpElement.h \
    Object/StatusBarVolume/StatusBarVolume.h \
    Object/StatusBarVolume/StatusBarVolumeToolTip.h \
    Object/MainWindowConsole/MainWindowConsole.h \
    Object/StatusBarInfo/SystemWordWraper.h \
    Object/SettingsPanel/SettingsPanelContainer.h \
    Object/SettingsPanel/SettingsPanel.h \
    Object/SettingsPanel/SettingsBasePanel.h \
    Object/SettingsPanel/SettingsToolBar.h \
    Object/Delegates/TableBoolDelegate.h \
    Object/Delegates/TableColorDelegate.h \
    Object/Delegates/TableConvertDateTimeDelegate.h \
    Object/Delegates/TableConvertDateDelegate.h \
    Object/SettingsPanel/SettingsAppLoader.h \
    Object/MdiArea/MdiSubWindowFilter.h \
    Object/MdiArea/MdiAreaFilter.h \
    Object/StatusBar/StatusBarFilter.h \
    Object/HelpDesigner/HelpDesigner.h \
    Object/HelpDesigner/HelpSection.h \
    Object/HelpDesigner/HelpWindow.h \
    Object/HelpDesigner/HelpTextEdit.h \
    Object/HelpDesigner/HelpSearchWidget.h \
    Object/SettingsPanel/SettingsToolBarProps.h \
    Object/Delegates/GeoCoordinateDelegate.h \
    Object/StatusBarInfo/MouseEventFilter.h \
    Object/ExternalMainWindow.h \
    Object/SystemGuiCoreNg.h \
    Object/ExternalDialog.h \

FORMS += \
    Object/MainWindow.ui \
    Object/SettingsPanel/SettingsGuiCoreWidget.ui \
    Object/StatusBarInfo/StatusBarMapWidgetToolTip.ui \
    Object/StatusBarInfo/StatusBarPlanningWidgetToolTip.ui \
    Object/WindowCreator/AboutComponentsWidget.ui \
    Object/BasePanel/BasePanelWidget.ui \
    Object/StatusBarInfo/StatusBarInfoWidgetData.ui \
    Object/StatusBarInfo/StatusBarInfoWidgetPopupWindow.ui \
    Object/StatusBarInfo/StatusBarInfoWidgetToolTip.ui \
    Object/StatusBarVolume/StatusBarVolumeToolTip.ui \
    Object/MainWindowConsole/MainWindowConsole.ui \
    Object/SettingsPanel/SettingsPanel.ui \
    Object/SettingsPanel/SettingsBasePanel.ui \
    Object/SettingsPanel/SettingsToolBar.ui \
    Object/SettingsPanel/SettingsAppLoader.ui \
    Object/HelpDesigner/HelpWindow.ui \
    Object/HelpDesigner/HelpSearchWidget.ui \
    Object/SettingsPanel/SettingsToolBarProps.ui \
    Object/WindowCreator/AboutSystemDialog.ui

RESOURCES += \
    Sources.qrc

DISTFILES += \
    ComponentSystemGuiCoreNg.json \
    ComponentSystemGuiCoreNg.xml
