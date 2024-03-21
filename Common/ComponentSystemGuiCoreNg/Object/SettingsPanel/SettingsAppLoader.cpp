#include "SettingsAppLoader.h"
#include "ui_SettingsAppLoader.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>

#define TITLE_LOAD_ERR  "Игнорировать ошибки загрузки"
#define TITLE_INIT_ERR  "Игнорировать ошибки инициализации"

SettingsAppLoader::SettingsAppLoader (QWidget *parent)
    : ISystemGuiCoreSettingsPanelWidget (parent)
    , ui (new Ui::SettingsAppLoader)
{
    ui->setupUi (this);
    _title = tr ("Graphics kernel loader");
    _icon = QIcon (":/icons/icons/application-loader-24.png");

    ui->checkBoxInitError->setText (tr ("Ignore initialization errors"));
    ui->checkBoxInitError->setIcon (QIcon (":/icons/icons/arrow-warning-init.png"));

    ui->checkBoxLoadError->setText (tr ("Ignore load errors"));
    ui->checkBoxLoadError->setIcon (QIcon (":/icons/icons/arrow-warning-load.png"));

    connect (ui->checkBoxInitError, &QCheckBox::toggled, this, &SettingsAppLoader::onCheck);
    connect (ui->checkBoxLoadError, &QCheckBox::toggled, this, &SettingsAppLoader::onCheck);
}

SettingsAppLoader::~SettingsAppLoader () {
    delete ui;
}

QString SettingsAppLoader::settingsTitle () const {
    return _title;
}

QIcon SettingsAppLoader::settingsIcon () const {
    return _icon;
}

bool SettingsAppLoader::isComplete () const {
    return this->isCompletePrivate();
}

void SettingsAppLoader::applySettings () {
    this->saveLastState();
}

void SettingsAppLoader::cancelSettings ()
{
    QMapIterator<QString, bool>i (_lastState);
    while (i.hasNext()) {
        i.next();

        if (i.key () == tr ("Ignore initialization errors"))
            ui->checkBoxInitError->setChecked(i.value());
        if (i.key () == tr ("Ignore load errors"))
            ui->checkBoxLoadError->setChecked(i.value());
    }
}

QMap<QString, QVariant> SettingsAppLoader::settingsList ()
{
    QMap<QString, QVariant> buffList;
    saveSettings ();
    return buffList;
}

void SettingsAppLoader::setSettingsList (const QMap<QString, QVariant> &settingsList)
{
    Q_UNUSED (settingsList)

    loadSettings();
    saveLastState();
}

void SettingsAppLoader::loadSettings ()
{
    // load...
    QSettings settings (qApp->organizationName (), qApp->applicationName ());
    settings.beginGroup ("/dialogLoadComponents");
    ui->checkBoxLoadError->setChecked(settings.value("ignoreLoadError", false).toBool());
    ui->checkBoxInitError->setChecked(settings.value("ignoreInitError", false).toBool());
    settings.endGroup();
}

void SettingsAppLoader::saveSettings()
{
    // save...
    QSettings settings (qApp->organizationName (), qApp->applicationName ());
    settings.beginGroup ("/dialogLoadComponents");
    settings.setValue("ignoreLoadError", ui->checkBoxLoadError->isChecked());
    settings.setValue("ignoreInitError", ui->checkBoxInitError->isChecked());
    settings.endGroup();
}

void SettingsAppLoader::saveLastState ()
{
    _lastState.insert(ui->checkBoxInitError->text(), ui->checkBoxInitError->isChecked());
    _lastState.insert(ui->checkBoxLoadError->text(), ui->checkBoxLoadError->isChecked());
}

bool SettingsAppLoader::isCompletePrivate () const
{
    if(_lastState.value(ui->checkBoxInitError->text()) != ui->checkBoxInitError->isChecked()
       || _lastState.value(ui->checkBoxLoadError->text()) != ui->checkBoxLoadError->isChecked())
        return true;

    return false;
}

void SettingsAppLoader::onCheck (bool isChecked)
{
    Q_UNUSED(isChecked)
    emit settingsChanged ();
}
