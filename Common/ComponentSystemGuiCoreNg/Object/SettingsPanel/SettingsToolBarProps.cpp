#include "SettingsToolBarProps.h"
#include "ui_SettingsToolBarProps.h"

SettingsToolBarProps::SettingsToolBarProps (SystemGuiCoreToolBar *toolBar, QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::SettingsToolBarProps)
{
    ui->setupUi (this);
    _toolBar = toolBar;

    ui->checkBoxGroup->setText (_toolBar->getTitle ());

    connect (ui->checkBoxGroup, &QCheckBox::toggled, this, &SettingsToolBarProps::onCheckGroup);
    connect (ui->checkBoxMovable, &QCheckBox::toggled, this, &SettingsToolBarProps::onCheck);
    connect (ui->_checkBoxShowText, &QCheckBox::toggled, this, [this]() {
        emit settingsChanged ();
    });
}

SettingsToolBarProps::~SettingsToolBarProps ()
{
    _toolBar = nullptr;
    delete ui;
}

QString SettingsToolBarProps::toolbarTitle () const {
    return _toolBar->getTitle ();
}

bool SettingsToolBarProps::isToolbarUsed () const {
    return ui->checkBoxGroup->isChecked();
}

bool SettingsToolBarProps::isToolbarMovable () const {
    return ui->checkBoxMovable->isChecked ();
}

bool SettingsToolBarProps::isToolBarShowText () const {
    return ui->_checkBoxShowText->isChecked ();
}

void SettingsToolBarProps::setIcon (const QIcon &icon) {
    ui->checkBoxGroup->setIcon (icon);
}

void SettingsToolBarProps::setToolbarUsed (bool isUsed) {
    ui->checkBoxGroup->setChecked(isUsed);
}

void SettingsToolBarProps::setToolbarMovable (bool isMovable) {
    ui->checkBoxMovable->setChecked (isMovable);
}

void SettingsToolBarProps::setToolBarShowText (bool value) {
    ui->_checkBoxShowText->setChecked (value);
}

bool SettingsToolBarProps::isComplete () const
{
    if (!_lastState.contains (ui->checkBoxGroup->objectName ())
            || !_lastState.contains (ui->checkBoxMovable->objectName ())
            || !_lastState.contains (ui->_checkBoxShowText->objectName ()))
        return false;

    if (_lastState.value (ui->checkBoxGroup->objectName ()) != ui->checkBoxGroup->isChecked ()
            || _lastState.value (ui->checkBoxMovable->objectName ()) != ui->checkBoxMovable->isChecked ()
            || _lastState.value (ui->_checkBoxShowText->objectName ()) != ui->_checkBoxShowText->isChecked ())
        return true;

    return false;
}

void SettingsToolBarProps::cancelSettings ()
{
    if (_lastState.contains (ui->checkBoxGroup->objectName ()))
        ui->checkBoxGroup->setChecked (_lastState.value (ui->checkBoxGroup->objectName ()));

    if (_lastState.contains (ui->checkBoxMovable->objectName ()))
        ui->checkBoxMovable->setChecked (_lastState.value (ui->checkBoxMovable->objectName ()));

    if (_lastState.contains (ui->_checkBoxShowText->objectName ()))
        ui->_checkBoxShowText->setChecked (_lastState.value (ui->_checkBoxShowText->objectName ()));
}

void SettingsToolBarProps::applySettings ()
{
    if (!_toolBar)
        return;

    _toolBar->setVisible (ui->checkBoxGroup->isChecked ());
    _toolBar->setMovable (ui->checkBoxMovable->isChecked ());

    if (ui->_checkBoxShowText->isChecked ())
        _toolBar->setToolButtonStyle (Qt::ToolButtonTextUnderIcon);
    else
        _toolBar->setToolButtonStyle (Qt::ToolButtonIconOnly);

    _toolBar->setFloatable (false);
    saveLastState ();
}

void SettingsToolBarProps::saveLastState ()
{
    _lastState.insert (ui->checkBoxGroup->objectName (), ui->checkBoxGroup->isChecked ());
    _lastState.insert (ui->checkBoxMovable->objectName (), ui->checkBoxMovable->isChecked ());
    _lastState.insert (ui->_checkBoxShowText->objectName (), ui->_checkBoxShowText->isChecked ());
}

void SettingsToolBarProps::onCheckGroup (bool isChecked)
{
    ui->checkBoxMovable->setEnabled (isChecked);
    emit settingsChanged ();
}

void SettingsToolBarProps::onCheck (bool isChecked)
{
    Q_UNUSED (isChecked)
    emit settingsChanged ();
}
