#include "SettingsMdiArea.h"
#include "ui_SettingsGuiCore.h"
#include <QtCore/QDebug>

#define TITLE_MODE_MDI    "Окна"
#define TITLE_MODE_TABS   "Вкладки"
#define TITLE_H_SCROLLBAR "Горизонтальный скроллбар"
#define TITLE_V_SCROLLBAR "Вертикальный скроллбар"

SettingsGuiCore::SettingsGuiCore (QMdiArea *mdiArea, QAction *tileAct, QAction *cascadeAct, QWidget *parent)
    : ISystemGuiCoreSettingsPanelWidget (parent)
    , ui (new Ui::SettingsGuiCore)
    , _mdiArea (mdiArea)
    , _tileAct (tileAct)
    , _cascadeAct (cascadeAct)
{
    ui->setupUi (this);

    _isLoad = false;
    _title = tr ("Workspace");
    _icon = QIcon (":/icons/icons/application-24.png");

    if (_mdiArea)
    {
        ui->radioButtonTabs->setText (tr (TITLE_MODE_TABS));
        ui->radioButtonTabs->setIcon (QIcon (":/icons/icons/ui-tab-content.png"));

        ui->radioButtonWindows->setText (tr (TITLE_MODE_MDI));
        ui->radioButtonWindows->setIcon (QIcon (":/icons/icons/application-sub.png"));
        ui->radioButtonWindows->setChecked (true);

        ui->checkBoxHScrollbar->setText (tr (TITLE_H_SCROLLBAR));
        ui->checkBoxHScrollbar->setIcon (QIcon (":/icons/icons/ui-scroll-pane-horizontal.png"));
        ui->checkBoxHScrollbar->setChecked (true);

        ui->checkBoxVScrollbar->setText (tr(TITLE_V_SCROLLBAR));
        ui->checkBoxVScrollbar->setIcon (QIcon(":/icons/icons/ui-scroll-pane.png"));
        ui->checkBoxVScrollbar->setChecked (true);

        connect (ui->radioButtonTabs, SIGNAL(toggled(bool)), this, SLOT(onCheck(bool)));
        connect (ui->radioButtonWindows, SIGNAL(toggled(bool)), this, SLOT(onCheck(bool)));
        connect (ui->checkBoxHScrollbar, SIGNAL(toggled(bool)), this, SLOT(onCheck(bool)));
        connect (ui->checkBoxVScrollbar, SIGNAL(toggled(bool)), this, SLOT(onCheck(bool)));
    }
    else
    {
        ui->_groupBoxMdi->setEnabled (true);
    }
}

SettingsGuiCore::~SettingsGuiCore () {
    delete ui;
}

QString SettingsGuiCore::settingsTitle () const {
    return _title;
}

QIcon SettingsGuiCore::settingsIcon () const  {
    return _icon;
}

bool SettingsGuiCore::isComplete () const {
    return isCompletePrivate ();
}

void SettingsGuiCore::applySettings ()
{
    if (_mdiArea)
    {
        // mdi
        if (_lastState.value(ui->radioButtonWindows->text()) != ui->radioButtonWindows->isChecked())
        {
            if (ui->radioButtonWindows->isChecked())
            {
                _tileAct->setEnabled (true);
                _cascadeAct->setEnabled (true);
                _mdiArea->setViewMode (QMdiArea::SubWindowView);

                if (!_isLoad)
                    _cascadeAct->trigger ();
            }
        }

        // tabs
        if (_lastState.value(ui->radioButtonTabs->text()) != ui->radioButtonTabs->isChecked())
        {
            if (ui->radioButtonTabs->isChecked())
            {
                _tileAct->setEnabled (false);
                _cascadeAct->setEnabled (false);
                _mdiArea->setViewMode (QMdiArea::TabbedView);

#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0))
                _mdiArea->setTabsClosable (true);
                _mdiArea->setTabsMovable (false);
#endif
            }
        }

        // scrollbars
        if (ui->checkBoxHScrollbar->isChecked ())
            _mdiArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAsNeeded);
        else
            _mdiArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);

        if (ui->checkBoxVScrollbar->isChecked ())
            _mdiArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
        else
            _mdiArea->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    }

    saveLastState ();
}

void SettingsGuiCore::cancelSettings ()
{
    QMapIterator<QString, bool> i(_lastState);
    while (i.hasNext ())
    {
        i.next ();

        if (_mdiArea)
        {
            // загружаем состояния QRadioButton-ов
            if (i.key() == tr(TITLE_MODE_MDI))
                ui->radioButtonWindows->setChecked(i.value());
            if (i.key() == tr(TITLE_MODE_TABS))
                ui->radioButtonTabs->setChecked(i.value());
            if (i.key() == TITLE_H_SCROLLBAR)
                ui->checkBoxHScrollbar->setChecked(i.value());
            if (i.key() == TITLE_V_SCROLLBAR)
                ui->checkBoxVScrollbar->setChecked(i.value());
        }
    }
}

QMap<QString, QVariant> SettingsGuiCore::settingsList ()
{
    QMap<QString, QVariant> buffList;

    if (_mdiArea)
    {
        // сохраняем состояния QRadioButton-ов
        buffList.insert(ui->radioButtonTabs->text(), ui->radioButtonTabs->isChecked());
        buffList.insert(ui->radioButtonWindows->text(), ui->radioButtonWindows->isChecked());
        buffList.insert(ui->checkBoxHScrollbar->text(), ui->checkBoxHScrollbar->isChecked());
        buffList.insert(ui->checkBoxVScrollbar->text(), ui->checkBoxVScrollbar->isChecked());
    }

    return buffList;
}

void SettingsGuiCore::setSettingsList (const QMap<QString, QVariant> &settingsList)
{
    if (!settingsList.isEmpty ())
    {
        QMapIterator<QString, QVariant> i(settingsList);
        while(i.hasNext())
        {
            i.next();

            if (_mdiArea)
            {
                // загружаем состояния QRadioButton-ов
                if (i.key() == TITLE_MODE_MDI)
                    ui->radioButtonWindows->setChecked(i.value().toBool());
                if (i.key() == TITLE_MODE_TABS)
                    ui->radioButtonTabs->setChecked(i.value().toBool());
                if (i.key() == TITLE_H_SCROLLBAR)
                    ui->checkBoxHScrollbar->setChecked(i.value().toBool());
                if (i.key() == TITLE_V_SCROLLBAR)
                    ui->checkBoxVScrollbar->setChecked(i.value().toBool());
            }
        }
    }

    _isLoad = true;
    applySettings ();
    _isLoad = false;
    saveLastState ();
}

void SettingsGuiCore::saveLastState()
{
    if (_mdiArea)
    {
        _lastState.insert (ui->radioButtonTabs->text (), ui->radioButtonTabs->isChecked ());
        _lastState.insert (ui->radioButtonWindows->text (), ui->radioButtonWindows->isChecked ());
        _lastState.insert (ui->checkBoxHScrollbar->text (), ui->checkBoxHScrollbar->isChecked ());
        _lastState.insert (ui->checkBoxVScrollbar->text (), ui->checkBoxVScrollbar->isChecked ());
    }
}

bool SettingsGuiCore::isCompletePrivate () const
{
    if (_mdiArea)
    {
        if (_lastState.value(ui->radioButtonTabs->text()) != ui->radioButtonTabs->isChecked()
                || _lastState.value(ui->radioButtonWindows->text()) != ui->radioButtonWindows->isChecked()
                || _lastState.value(ui->checkBoxHScrollbar->text()) != ui->checkBoxHScrollbar->isChecked()
                || _lastState.value(ui->checkBoxVScrollbar->text()) != ui->checkBoxVScrollbar->isChecked())
            return true;
    }

    return false;
}

void SettingsGuiCore::onCheck (bool isChecked)
{
    Q_UNUSED(isChecked)
    emit settingsChanged ();
}
