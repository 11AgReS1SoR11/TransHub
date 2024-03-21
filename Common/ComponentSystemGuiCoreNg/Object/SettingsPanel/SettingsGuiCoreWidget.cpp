#include "SettingsGuiCoreWidget.h"
#include "ui_SettingsGuiCoreWidget.h"
#include <QtCore/QDebug>

namespace GuiCoreSettings
{
static const char reopen_windows[] = "reopen_windows_after_restart";
}

SettingsGuiCoreWidget::SettingsGuiCoreWidget (ISystemGuiCoreSettings *settings,
                                              QMdiArea *mdiArea, QAction *tileAct,
                                              QAction *cascadeAct, QWidget *parent)
    : ISystemGuiCoreSettingsPanelWidget (parent)
    , ui (new Ui::SettingsGuiCoreWidget)
    , _settings (settings)
    , _mdiArea (mdiArea)
    , _tileAct (tileAct)
    , _cascadeAct (cascadeAct)
{
    ui->setupUi (this);
    //setAttribute (Qt::WA_DeleteOnClose);
    //-- в процессе доработки gorbachev
    ui->_checkBoxOpenWidgets->setEnabled (false);

    _isLoad = false;
    _title = tr ("Workspace");
    _icon = QIcon (":/icons/icons/application-24.png");

    if (_mdiArea)
    {
        ui->_rbtnTabs->setIcon (QIcon (":/icons/icons/ui-tab-content.png"));
        ui->_rbtnWindows->setIcon (QIcon (":/icons/icons/application-sub.png"));
        ui->_rbtnWindows->setChecked (true);

        ui->_checkBoxHScrollbar->setIcon (QIcon (":/icons/icons/ui-scroll-pane-horizontal.png"));
        ui->_checkBoxHScrollbar->setChecked (true);

        ui->_checkBoxVScrollbar->setIcon (QIcon(":/icons/icons/ui-scroll-pane.png"));
        ui->_checkBoxVScrollbar->setChecked (true);

        connect (ui->_rbtnTabs, &QRadioButton::toggled, this, [this](){ emit settingsChanged (); });
        connect (ui->_rbtnWindows, &QRadioButton::toggled, this, [this](){ emit settingsChanged (); });
        connect (ui->_checkBoxHScrollbar, &QCheckBox::toggled, this, [this](){ emit settingsChanged (); });
        connect (ui->_checkBoxVScrollbar, &QCheckBox::toggled, this, [this](){ emit settingsChanged (); });
        connect (ui->_checkBoxOpenWidgets, &QCheckBox::toggled, this, [this](){ emit settingsChanged (); });
    }
    else
    {
        ui->_groupBoxMdi->setEnabled (true);
    }

    if (_settings)
    {
        ui->_checkBoxToolButtonTextVisible->setChecked (_settings->toolButtonTexVisible ());
        connect (ui->_checkBoxToolButtonTextVisible, &QCheckBox::stateChanged, this, [this](){ emit settingsChanged (); });

        ui->_checkBoxStatusBarVisible->setChecked (_settings->statusBarVisible ());
        connect (ui->_checkBoxStatusBarVisible, &QCheckBox::stateChanged, this, [this](){ emit settingsChanged (); });
    }
}

SettingsGuiCoreWidget::~SettingsGuiCoreWidget () {
    delete ui;
}

QString SettingsGuiCoreWidget::settingsTitle () const {
    return _title;
}

QIcon SettingsGuiCoreWidget::settingsIcon () const  {
    return _icon;
}

bool SettingsGuiCoreWidget::isComplete () const
{
    if (_mdiArea)
    {
        if (_lastState.value (ui->_rbtnTabs->text ()) != ui->_rbtnTabs->isChecked ()
                || _lastState.value (ui->_rbtnWindows->text ()) != ui->_rbtnWindows->isChecked ()
                || _lastState.value (ui->_checkBoxHScrollbar->text ()) != ui->_checkBoxHScrollbar->isChecked ()
                || _lastState.value (ui->_checkBoxVScrollbar->text ()) != ui->_checkBoxVScrollbar->isChecked ()
                || _lastState.value (GuiCoreSettings::reopen_windows) != ui->_checkBoxOpenWidgets->isChecked ()
                )
            return true;
    }

    if (_settings)
    {
        if (_lastState.value (ui->_checkBoxStatusBarVisible->text ()) != ui->_checkBoxStatusBarVisible->isChecked ()
                || _lastState.value (ui->_checkBoxToolButtonTextVisible->text ()) != ui->_checkBoxToolButtonTextVisible->isChecked ())
            return true;
    }

    return false;
}

void SettingsGuiCoreWidget::applySettings ()
{
    if (_mdiArea)
    {
        //-- mdi
        if (_lastState.value (ui->_rbtnWindows->text ()) != ui->_rbtnWindows->isChecked ())
        {
            if (ui->_rbtnWindows->isChecked ())
            {
                _tileAct->setEnabled (true);
                _cascadeAct->setEnabled (true);
                _mdiArea->setViewMode (QMdiArea::SubWindowView);

                if (!_isLoad)
                    _cascadeAct->trigger ();
            }
        }

        //-- tabs
        if (_lastState.value (ui->_rbtnTabs->text ()) != ui->_rbtnTabs->isChecked ())
        {
            if (ui->_rbtnTabs->isChecked ())
            {
                _tileAct->setEnabled (false);
                _cascadeAct->setEnabled (false);
                _mdiArea->setViewMode (QMdiArea::TabbedView);

                _mdiArea->setTabsClosable (true);
                _mdiArea->setTabsMovable (false);
            }
        }

        //-- scrollbars
        if (ui->_checkBoxHScrollbar->isChecked ())
            _mdiArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAsNeeded);
        else
            _mdiArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);

        if (ui->_checkBoxVScrollbar->isChecked ())
            _mdiArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
        else
            _mdiArea->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    }

    if (_settings) {
        _settings->setStatusBarVisible (ui->_checkBoxStatusBarVisible->isChecked ());
        _settings->setToolButtonTexVisible (ui->_checkBoxToolButtonTextVisible->isChecked ());
    }

    saveLastState ();
}

void SettingsGuiCoreWidget::cancelSettings ()
{
    QMapIterator<QString, bool> i (_lastState);
    while (i.hasNext ())
    {
        i.next ();

        if (_mdiArea)
        {
            //-- загружаем состояния QRadioButton-ов
            if (i.key () == ui->_rbtnWindows->text())   //tr ("Windows")
                ui->_rbtnWindows->setChecked (i.value ());
            if (i.key () == ui->_rbtnTabs->text())      //tr ("Tabs"))
                ui->_rbtnTabs->setChecked (i.value ());
            if (i.key () == ui->_checkBoxHScrollbar->text()) //tr ("Horizontal scrollbar"))
                ui->_checkBoxHScrollbar->setChecked (i.value ());
            if (i.key () == ui->_checkBoxVScrollbar->text()) //tr ("Vertical scrollbar"))
                ui->_checkBoxVScrollbar->setChecked (i.value ());
            if (i.key () == GuiCoreSettings::reopen_windows)
                ui->_checkBoxOpenWidgets->setChecked (i.value ());
        }
    }
}

QMap<QString, QVariant> SettingsGuiCoreWidget::settingsList ()
{
    QMap<QString, QVariant> buffList;

    if (_mdiArea)
    {
        //-- сохраняем состояния QRadioButton-ов
        buffList.insert (ui->_rbtnTabs->text (), ui->_rbtnTabs->isChecked ());
        buffList.insert (ui->_rbtnWindows->text (), ui->_rbtnWindows->isChecked ());
        buffList.insert (ui->_checkBoxHScrollbar->text (), ui->_checkBoxHScrollbar->isChecked ());
        buffList.insert (ui->_checkBoxVScrollbar->text (), ui->_checkBoxVScrollbar->isChecked ());
        buffList.insert (GuiCoreSettings::reopen_windows, ui->_checkBoxOpenWidgets->isChecked ());
    }

    return buffList;
}

void SettingsGuiCoreWidget::setSettingsList (const QMap<QString, QVariant> &settingsList)
{
    if (!settingsList.isEmpty ())
    {
        QMapIterator<QString, QVariant> i (settingsList);
        while (i.hasNext ())
        {
            i.next ();

            if (_mdiArea)
            {
                //-- загружаем состояния QRadioButton-ов
                if (i.key () == ui->_rbtnWindows->text ())
                    ui->_rbtnWindows->setChecked (i.value ().toBool ());
                if (i.key () == ui->_rbtnTabs->text ())
                    ui->_rbtnTabs->setChecked (i.value ().toBool ());
                if (i.key () == ui->_checkBoxHScrollbar->text ())
                    ui->_checkBoxHScrollbar->setChecked (i.value ().toBool ());
                if (i.key () == ui->_checkBoxVScrollbar->text ())
                    ui->_checkBoxVScrollbar->setChecked (i.value ().toBool ());
                if (i.key () == GuiCoreSettings::reopen_windows)
                    ui->_checkBoxOpenWidgets->setChecked (i.value ().toBool ());
            }
        }
    }

    _isLoad = true;
    applySettings ();
    _isLoad = false;
    saveLastState ();
}

void SettingsGuiCoreWidget::saveLastState ()
{
    if (_mdiArea) {
        _lastState.insert (ui->_rbtnTabs->text (), ui->_rbtnTabs->isChecked ());
        _lastState.insert (ui->_rbtnWindows->text (), ui->_rbtnWindows->isChecked ());
        _lastState.insert (ui->_checkBoxHScrollbar->text (), ui->_checkBoxHScrollbar->isChecked ());
        _lastState.insert (ui->_checkBoxVScrollbar->text (), ui->_checkBoxVScrollbar->isChecked ());
        _lastState.insert (GuiCoreSettings::reopen_windows, ui->_checkBoxOpenWidgets->isChecked ());
    }

    _lastState.insert (ui->_checkBoxStatusBarVisible->text (), ui->_checkBoxStatusBarVisible->isChecked ());
    _lastState.insert (ui->_checkBoxToolButtonTextVisible->text (), ui->_checkBoxToolButtonTextVisible->isChecked ());
}
