#include "SystemGuiCoreSettings.h"
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

SystemGuiCoreSettings::SystemGuiCoreSettings (QObject *parent)
    : ISystemGuiCoreSettings (parent)
{}

SystemGuiCoreSettings::~SystemGuiCoreSettings ()
{}

void SystemGuiCoreSettings::setSettingsProperty (SettinsProperty property, const QVariant &value)
{
    QSettings settings (qApp->organizationName (), qApp->applicationName ());

    if (ToolButtonTextVisible == property) {
        settings.beginGroup ("ComponentSystemGuiCoreNg");
        if (settings.value ("toolBarTextVisible").toBool () != value.toBool ()) {
            settings.setValue ("toolBarTextVisible", value);
            emit settingsChanged (ToolButtonTextVisible, value);
        }
    }
    else if (StatusBarVisible == property) {
        settings.beginGroup ("ComponentSystemGuiCoreNg");
        if (settings.value ("statusBarVisible").toBool () != value.toBool ()) {
            settings.setValue ("statusBarVisible", value);
            emit settingsChanged (StatusBarVisible, value);
        }
    }
    else
        qWarning () << "[SystemGuiCoreSettings][setSettingsProperty] Unknow settings property";
}

void SystemGuiCoreSettings::setSettingsProperty (const QList<QString> &groups, const QString &key, const QVariant &value)
{
    QSettings settings (qApp->organizationName (), qApp->applicationName ());
    for (const QString &group : qAsConst (groups))
        settings.beginGroup (group);
    settings.setValue (key, value);

    emit settingsChanged (groups, key, value);
}

QVariant SystemGuiCoreSettings::settingsProperty (SettinsProperty property)
{
    QSettings settings (qApp->organizationName (), qApp->applicationName ());

    if (ToolButtonTextVisible == property) {
        settings.beginGroup ("ComponentSystemGuiCoreNg");
        return settings.value ("toolBarTextVisible", false).toBool ();
    }
    else if (StatusBarVisible == property) {
        settings.beginGroup ("ComponentSystemGuiCoreNg");
        return settings.value ("statusBarVisible", false).toBool ();
    }
    else {
        qWarning () << "[SystemGuiCoreSettings][settingsProperty] Unknow settings property";
        return QVariant ();
    }
}

QVariant SystemGuiCoreSettings::settingsProperty (const QList<QString> &groups, const QString &key)
{
    QSettings settings (qApp->organizationName (), qApp->applicationName ());
    for (const QString &group : qAsConst (groups))
        settings.beginGroup (group);
    return settings.value (key);
}
