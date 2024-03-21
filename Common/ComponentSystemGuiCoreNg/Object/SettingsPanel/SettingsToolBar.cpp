#include "SettingsToolBar.h"
#include "ui_SettingsToolBar.h"

SettingsToolBar::SettingsToolBar (const QList<SystemGuiCoreToolBar *> &toolbarsList, QWidget *parent)
    : ISystemGuiCoreSettingsPanelWidget (parent)
    , ui (new Ui::SettingsToolBar)
{
    ui->setupUi (this);

    _title = tr ("Settings toolbars");
    _icon = QIcon (":/icons/icons/application-block-24.png");

    for (int i = 0; i < toolbarsList.size(); i++) {
        auto stb = new SettingsToolBarProps (toolbarsList[i], this);
        stb->setToolbarUsed (true);
        stb->setToolbarMovable (true);
        stb->setIcon (QIcon (":/icons/icons/application-block.png"));

        ui->verticalLayoutForWidgets->addWidget (stb);
        _toolbars.insert (stb->toolbarTitle (), stb);
        _toolbarsWidgets.insert(stb->toolbarTitle (), toolbarsList [i]);
        connect (stb, &SettingsToolBarProps::settingsChanged, this, &SettingsToolBar::settingsChanged);
    }
    ui->verticalLayoutForWidgets->update ();
}

SettingsToolBar::~SettingsToolBar ()
{
    _toolbars.clear ();
    _toolbarsWidgets.clear ();
    delete ui;
}

QString SettingsToolBar::settingsTitle () const {
    return _title;
}

QIcon SettingsToolBar::settingsIcon () const {
    return _icon;
}

bool SettingsToolBar::isComplete () const {
    return isCompletePrivate ();
}

void SettingsToolBar::applySettings ()
{
    QMapIterator<QString, SettingsToolBarProps*> i (_toolbars);
    while (i.hasNext ()) {
        i.next ();
        i.value ()->applySettings ();
    }
}

void SettingsToolBar::cancelSettings ()
{
    QMapIterator<QString, SettingsToolBarProps*>i (_toolbars);
    while (i.hasNext ()) {
        i.next ();
        i.value ()->cancelSettings ();
    }
}

QMap<QString, QVariant> SettingsToolBar::settingsList ()
{
    QMap<QString, QVariant> list;

    //-- сохранение состояния QCheckBox-ов
    QMapIterator<QString, SettingsToolBarProps*> i (_toolbars);
    while (i.hasNext ()) {
        i.next ();
        list.insert (QString ("%1_isUsed").arg (i.key ()), i.value ()->isToolbarUsed ());
        list.insert (QString ("%1_isMovable").arg (i.key ()), i.value ()->isToolbarMovable ());
        list.insert (QString ("%1_isShowText").arg (i.key ()), i.value ()->isToolBarShowText ());
    }

    return list;
}

void SettingsToolBar::setSettingsList (const QMap<QString, QVariant> &settingsList)
{
    QMapIterator<QString, QVariant> iter (settingsList);
    while (iter.hasNext ())
    {
        iter.next ();

        //-- загружаем состояния Toolbar-ов
        if (iter.key ().indexOf ("_isUsed") != -1) {
            QString buffName = iter.key ();
            buffName = buffName.replace ("_isUsed", "");
            if (_toolbars.contains (buffName))
                _toolbars.value (buffName)->setToolbarUsed (iter.value ().toBool ());
        }
        else if (iter.key ().indexOf ("_isMovable") != -1) {
            QString buffName = iter.key ();
            buffName = buffName.replace ("_isMovable", "");
            if (_toolbars.contains (buffName))
                _toolbars.value (buffName)->setToolbarMovable (iter.value ().toBool ());
        }
        else if (iter.key ().indexOf ("_isShowText") != -1) {
            QString buffName = iter.key ();
            buffName = buffName.replace ("_isShowText", "");
            if (_toolbars.contains (buffName))
                _toolbars.value (buffName)->setToolBarShowText (iter.value ().toBool ());
        }
    }

    applySettings ();
}

bool SettingsToolBar::isCompletePrivate () const
{
    QMapIterator<QString, SettingsToolBarProps*>i(_toolbars);
    while (i.hasNext ()) {
        i.next();
        if (i.value()->isComplete ())
            return true;
    }
    return false;
}
