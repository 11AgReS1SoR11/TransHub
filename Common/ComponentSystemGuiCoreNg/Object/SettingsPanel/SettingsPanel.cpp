#include "SettingsPanel.h"
#include "ui_SettingsPanel.h"

#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>

SettingsPanel::SettingsPanel (SettingsPanelContainer *panelContainer, QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::SettingsPanel)
    , _panelContainer (panelContainer)
{
    ui->setupUi (this);
    setAttribute (Qt::WA_DeleteOnClose);

    setWindowTitle (tr ("Options"));
    setWindowIcon (QIcon (":/icons/icons/"));

    initGui ();
}

SettingsPanel::~SettingsPanel ()
{
    if (!_panelContainer.isNull ()) {
        for(int i = 0; i < _settingsWidgets.size(); i++) {
            ui->stackedWidget->removeWidget(_settingsWidgets[i]);
            _settingsWidgets[i]->setParent(nullptr);
        }
        _settingsWidgets.clear();
    }
    delete ui;
}

void SettingsPanel::initGui()
{
    if(!_panelContainer) {
        qCritical() << "[SettingsPanel][initGui] Panel container is NULL! Init settings panel gui failed!";
        return;
    }

    _settingsWidgets = _panelContainer.data()->settingsPanels();
    for(int i = 0; i < _settingsWidgets.size(); i++) {
        ISystemGuiCoreSettingsPanelWidget* buffPanelW = _settingsWidgets[i];
        QListWidgetItem* item = new QListWidgetItem(buffPanelW->settingsIcon(), buffPanelW->settingsTitle());
        item->setToolTip(buffPanelW->settingsTitle());
        ui->listWidget->addItem(item);
        ui->stackedWidget->addWidget(buffPanelW);

        connect(buffPanelW, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));
    }

    ui->_btnSave->setIcon(QIcon(":/icons/icons/tick-circle.png"));
    ui->_btnSave->setEnabled(false);

    ui->_btnCancel->setIcon(QIcon(":/icons/icons/icon_cancel.png"));
    ui->_btnCancel->setEnabled(true);

    ui->pushButtonOk->setIcon(QIcon(":/icons/icons/tick-white.png"));
    ui->pushButtonOk->setEnabled(false);

    ui->labelSearchIcon->setPixmap(QPixmap(":/icons/icons/magnifier.png"));
    ui->lineEditSearch->setToolTip(tr("Поиск раздела настроек"));

    connect (ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(currentRowChanged(int)));
    connect (ui->listWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(itemActivated(QListWidgetItem*)));
    connect (ui->lineEditSearch, SIGNAL(textChanged(QString)), this, SLOT(searchSettings(QString)));

    connect (ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(onOk()));
    connect (ui->_btnSave, SIGNAL(clicked()), this, SLOT(onApply()));
    connect (ui->_btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));

    connect (this, SIGNAL(destroyed()), this, SLOT(close()));

    ui->listWidget->setCurrentRow (0);
}

bool SettingsPanel::isCompleteAtLeastOne ()
{
    for(int i = 0; i < _settingsWidgets.size(); i++) {
        if(_settingsWidgets[i]->isComplete())
            return true;
    }
    return false;
}

void SettingsPanel::privateOk()
{
    for(int i = 0; i < _settingsWidgets.size(); i++) {
        if(_settingsWidgets[i]->isComplete())
            _settingsWidgets[i]->applySettings();
    }
    ui->pushButtonOk->setEnabled(false);
    ui->_btnCancel->setText (tr ("Close"));
}

void SettingsPanel::privateApply(ISystemGuiCoreSettingsPanelWidget *panelWidget)
{
    panelWidget->applySettings();
    ui->_btnSave->setEnabled(false);
    ui->pushButtonOk->setEnabled(this->isCompleteAtLeastOne());

    if(!ui->pushButtonOk->isEnabled())
        ui->_btnCancel->setText (tr ("Close"));
}

void SettingsPanel::privateCancel()
{
    for (int i = 0; i < _settingsWidgets.size (); i++)
        _settingsWidgets[i]->cancelSettings ();

    ui->_btnCancel->setText (tr ("Close"));
}

void SettingsPanel::closeEvent (QCloseEvent *event)
{
    if (this->isCompleteAtLeastOne ()) {
        QMessageBox msgBox;
        msgBox.setText ("Имеются не сохраненные настройки! Сохранить их?");
        msgBox.setWindowTitle ("Завершение настроек");
        msgBox.setIcon (QMessageBox::Information);
        msgBox.setStandardButtons (QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton (QMessageBox::Save);
        int ret = msgBox.exec ();

        switch (ret) {
           case QMessageBox::Save:
               this->privateOk();
               break;
           case QMessageBox::Cancel:
               this->privateCancel();
               break;
           default:
               break;
         }
    }
    event->accept();
}

void SettingsPanel::currentRowChanged(int row)
{
    QListWidgetItem* buffItem = this->ui->listWidget->item(row);
    this->itemActivated(buffItem);
}

void SettingsPanel::itemActivated(QListWidgetItem *item)
{
    ui->labelSettingsName->setText(item->text());
    ui->stackedWidget->setCurrentIndex(ui->listWidget->currentRow());

    ISystemGuiCoreSettingsPanelWidget* panelW =
            dynamic_cast<ISystemGuiCoreSettingsPanelWidget*>(ui->stackedWidget->currentWidget());
    if(!panelW)
        return;

    ui->_btnSave->setEnabled(panelW->isComplete());
}

void SettingsPanel::settingsChanged ()
{
    ISystemGuiCoreSettingsPanelWidget* panelW = dynamic_cast<ISystemGuiCoreSettingsPanelWidget*>(sender());
    if (!panelW)
        return;

    ui->_btnSave->setEnabled (panelW->isComplete ());
    ui->pushButtonOk->setEnabled (isCompleteAtLeastOne ());

    if (ui->pushButtonOk->isEnabled ())
        ui->_btnCancel->setText (tr ("Cancel"));
    else
        ui->_btnCancel->setText (tr ("Close"));
}

void SettingsPanel::searchSettings (const QString &title)
{
    if (title.isEmpty ()) {
        ui->listWidget->scrollToTop ();
        ui->listWidget->setCurrentRow (0);
        return;
    }

    QList<QListWidgetItem*> findItems = ui->listWidget->findItems (title, Qt::MatchContains);
    if (findItems.size () != 0) {
        QListWidgetItem* buffItem = findItems[0];
        buffItem->setSelected (true);
        ui->listWidget->scrollToItem (buffItem);
    }
}

void SettingsPanel::onOk ()
{
    privateOk ();
    emit destroyed ();
}

void SettingsPanel::onApply ()
{
    auto panelW = dynamic_cast<ISystemGuiCoreSettingsPanelWidget*> (ui->stackedWidget->currentWidget ());
    if (!panelW)
        return;

    privateApply (panelW);
}

void SettingsPanel::onCancel ()
{
    privateCancel ();
    emit destroyed ();
}
