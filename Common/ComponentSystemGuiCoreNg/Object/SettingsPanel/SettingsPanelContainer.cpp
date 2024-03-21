#include "SettingsPanelContainer.h"
#include <QtCore/QDebug>

SettingsPanelContainer::SettingsPanelContainer (QObject *parent)
    : QObject (parent)
{}

SettingsPanelContainer::~SettingsPanelContainer ()
{
    QMapIterator<QString, ISystemGuiCoreSettingsPanelWidget*>i (_widgets);
    while (i.hasNext ()) {
        i.next ();
        auto wid = i.value ();
        if (wid)
            delete wid;
    }
    _widgets.clear ();
}

bool SettingsPanelContainer::addWidget (ISystemGuiCoreSettingsPanelWidget *widget)
{
    if (!widget) {
        qCritical() << "[SettingsPanelContainer][addWidget] Widget is NULL! Add failed!";
        return false;
    }
    if (widget->settingsTitle ().isEmpty ()) {
        qCritical() << "[SettingsPanelContainer][addWidget] Widget title is Empty! Add failed!";
        return false;
    }

    if (_widgets.contains (widget->settingsTitle ())) {
        qCritical () << QString ("[SettingsPanelContainer][addWidget] Widget with title \"%1\" already added! Add failed!")
                       .arg (widget->settingsTitle());
        return false;
    }

    _widgets.insert (widget->settingsTitle (), widget);
    return true;
}

bool SettingsPanelContainer::removeWidget (ISystemGuiCoreSettingsPanelWidget *widget)
{
    if (!widget) {
        qCritical() << "[SettingsPanelContainer][removeWidget] Widget is NULL! Remove failed!";
        return false;
    }

    QMapIterator<QString, ISystemGuiCoreSettingsPanelWidget*> iter (_widgets);
    bool found = false;
    while (iter.hasNext () && !found) {
        iter.next ();
        if (iter.value () == widget)
        {
            _widgets.remove (iter.key ());
            found = true;
        }
    }

    return found;
}

QList<ISystemGuiCoreSettingsPanelWidget *> SettingsPanelContainer::settingsPanels () const {
    return _widgets.values ();
}

void SettingsPanelContainer::loadSettings (QSettings *settings)
{
    QMapIterator<QString, ISystemGuiCoreSettingsPanelWidget*>i(_widgets);
    while (i.hasNext())
    {
        i.next();

        ISystemGuiCoreSettingsPanelWidget* buffW = i.value ();
        settings->beginGroup (QString ("/SettingsPanel_%1").arg (buffW->settingsTitle ()));

        QMap<QString, QVariant> settingsMap;
        auto list = settings->allKeys ();
        for (const QString &key : qAsConst (list))
            settingsMap.insert (key, settings->value (key));

        settings->endGroup ();
        buffW->setSettingsList (settingsMap);
    }
}

void SettingsPanelContainer::saveSettings (QSettings *settings)
{
    QMapIterator<QString, ISystemGuiCoreSettingsPanelWidget*> i (_widgets);
    while (i.hasNext())
    {
        i.next ();

        ISystemGuiCoreSettingsPanelWidget* buffW = i.value ();
        settings->beginGroup (QString ("/SettingsPanel_%1").arg (buffW->settingsTitle ()));

        //-- чистим все в данной ветке
        auto list = settings->allKeys ();
        for (const QString &key : qAsConst (list))
            settings->remove (key);

        QMap<QString, QVariant> settingsMap = buffW->settingsList ();
        QMapIterator<QString, QVariant> k (settingsMap);
        while (k.hasNext()) {
            k.next ();
            settings->setValue (k.key (), k.value ());
        }
        settings->endGroup ();
    }
}
