#include "SettingsBasePanel.h"
#include "ui_SettingsBasePanel.h"

#include <QtCore/QDebug>

#define TITLE_SHOW_PANEL    "Показывать панель"
#define TITLE_FIXED_SIZE    "Фиксировать размер"
#define TITLE_AUTO_HIDE     "Автоматически скрывать"
#define TITLE_TABS_MOVABLE  "Перетаскивание вкладок"

#define TITLE_TABS_DEFAULT  "По умолчанию"
#define TITLE_TABS_TOP      "По верхнему краю"
#define TITLE_TABS_BOTTOM   "По нижнему краю"
#define TITLE_TABS_LEFT     "По левому краю"
#define TITLE_TABS_RIGHT    "По правому краю"

SettingsBasePanel::SettingsBasePanel(BasePanelDockWidget *basePanelDockWidget,
                                     BasePanelWidget *basePanelWidget,
                                     QWidget *parent) :
    ISystemGuiCoreSettingsPanelWidget(parent),
    ui(new Ui::SettingsBasePanel),
    _basePanelDockWidget(basePanelDockWidget),
    _basePanelWidget(basePanelWidget)
{
    ui->setupUi(this);

    _size = 0;
    _tabsPos = ISystemGuiCoreBasePanel::BasePanelTabDefault;

    ui->checkBoxAutoHide->setText(tr(TITLE_AUTO_HIDE));
    ui->checkBoxAutoHide->setIcon(QIcon(":/icons/icons/application-resize.png"));

    ui->checkBoxFixedSize->setText(tr(TITLE_FIXED_SIZE));
    ui->checkBoxFixedSize->setIcon(QIcon(":/icons/icons/application-resize-actual.png"));

    ui->checkBoxShowPanel->setText(tr(TITLE_SHOW_PANEL));

    ui->checkBoxTabsMovable->setText(tr(TITLE_TABS_MOVABLE));
    ui->checkBoxTabsMovable->setIcon(QIcon(":/icons/icons/ui-tab--arrow.png"));

    ui->checkBoxShowPanel->setChecked(true);
    ui->checkBoxFixedSize->setChecked(false);
    ui->checkBoxAutoHide->setChecked(false);

    ui->comboBoxTabsPosition->addItem(tr(TITLE_TABS_DEFAULT));
    ui->comboBoxTabsPosition->addItem(tr(TITLE_TABS_TOP));
    ui->comboBoxTabsPosition->addItem(tr(TITLE_TABS_BOTTOM));
    ui->comboBoxTabsPosition->addItem(tr(TITLE_TABS_LEFT));
    ui->comboBoxTabsPosition->addItem(tr(TITLE_TABS_RIGHT));

    connect(ui->checkBoxShowPanel, SIGNAL(toggled(bool)), this, SLOT(onCheck(bool)));
    connect(ui->checkBoxFixedSize, SIGNAL(toggled(bool)), this, SLOT(onCheck(bool)));
    connect(ui->checkBoxAutoHide, SIGNAL(toggled(bool)), this, SLOT(onCheck(bool))); // not supported in this version
    connect(ui->checkBoxTabsMovable, SIGNAL(toggled(bool)), this, SLOT(onCheck(bool)));

    connect(ui->comboBoxTabsPosition, SIGNAL(currentIndexChanged(QString)), this, SLOT(onTapsPosChanged(QString)));
}

SettingsBasePanel::~SettingsBasePanel()
{
    _basePanelDockWidget = nullptr;
    _basePanelWidget = nullptr;
    _allTabs.clear();
    delete ui;
}

bool SettingsBasePanel::initGui()
{
    if(!_basePanelDockWidget
       || !_basePanelWidget)
        return false;

    switch (_basePanelWidget->panelType()) {
        case ISystemGuiCoreBasePanel::BasePanelLeft: {
            _title = tr("Левая информационная панель");
            _icon = QIcon(":/icons/icons/application-sidebar-left-24.png");
            ui->checkBoxShowPanel->setIcon(QIcon(":/icons/icons/application-dock-180.png"));
            break;
        }
        case ISystemGuiCoreBasePanel::BasePanelRight: {
            _title = tr("Правая информационная панель");
            _icon = QIcon(":/icons/icons/application-sidebar-right-24.png");
            ui->checkBoxShowPanel->setIcon(QIcon(":/icons/icons/application-dock.png"));
            break;
        }
        case ISystemGuiCoreBasePanel::BasePanelBottom: {
            _title = tr("Нижняя информационная панель");
            _icon = QIcon(":/icons/icons/application-sidebar-bottom-24.png");
            ui->checkBoxShowPanel->setIcon(QIcon(":/icons/icons/application-dock-270.png"));
            break;
        }
        default: {
            qCritical() << "[SettingsBasePanel][initGui] Undefined panel type! Init settings base panel failed!";
            return false;
        }
    }

    _basePanelWidget->initTabs();
    QTabWidget* buffTabWidget = _basePanelWidget->getTabWidget();
    for(int i = 0; i < buffTabWidget->count(); i++) {
        QIcon tabIcon;
        QString tabName = buffTabWidget->tabToolTip(i);
        if(tabName.isEmpty())
            tabName = buffTabWidget->tabText(i);

        if(_basePanelWidget->tabPosition() == ISystemGuiCoreBasePanel::BasePanelTabWest)
            tabIcon = this->rotateIcon(buffTabWidget->tabIcon(i));
        else if(_basePanelWidget->tabPosition() == ISystemGuiCoreBasePanel::BasePanelTabEast)
            tabIcon = this->rotateIcon(buffTabWidget->tabIcon(i), -270);
        else
            tabIcon = buffTabWidget->tabIcon(i);

        QCheckBox* buffCheck = new QCheckBox(tabName, this);
        buffCheck->setIcon(tabIcon);
        buffCheck->setChecked(true);

        _allTabs.insert(tabName, buffCheck);
        ui->verticalLayoutForWidgets->addWidget(buffCheck);

        connect(buffCheck, SIGNAL(toggled(bool)), this, SLOT(onCheck(bool)));
    }
    ui->verticalLayoutForWidgets->update();
    ui->comboBoxTabsPosition->setCurrentIndex(0);

    _tabsPos = _basePanelWidget->tabPosition();
    ui->checkBoxTabsMovable->setChecked(buffTabWidget->isMovable());

    return true;
}

QString SettingsBasePanel::settingsTitle() const
{
    return _title;
}

QIcon SettingsBasePanel::settingsIcon() const
{
    return _icon;
}

bool SettingsBasePanel::isComplete() const
{
    return this->isCompletePrivate();
}

void SettingsBasePanel::applySettings()
{
    _basePanelDockWidget->setPanelVisible(ui->checkBoxShowPanel->isChecked());

    // если панель скрыта, то отключаем авто. сворачивание и выставляем флаг
    if(!ui->checkBoxShowPanel->isChecked()) {
        _basePanelDockWidget->setAutoHide(false);
        _basePanelDockWidget->setPanelState(BasePanelDockWidget::MoveInvisible);
        this->saveLastState();
        return;
    }

    if(_basePanelWidget->panelType() == ISystemGuiCoreBasePanel::BasePanelLeft
       || _basePanelWidget->panelType() == ISystemGuiCoreBasePanel::BasePanelRight) {
        if(ui->checkBoxFixedSize->isChecked()) {
            if(_basePanelDockWidget->panelState() == BasePanelDockWidget::MoveVisible)
                _size = _basePanelDockWidget->width();

            _basePanelDockWidget->setFixedWidth(_size);
        } else {
            _basePanelDockWidget->setFixedWidth(QWIDGETSIZE_MAX);
        }
    } else {
        if(ui->checkBoxFixedSize->isChecked()) {
            if(_basePanelDockWidget->panelState() == BasePanelDockWidget::MoveVisible)
                _size = _basePanelDockWidget->height();

            _basePanelDockWidget->setFixedHeight(_size);
        } else {
            _basePanelDockWidget->setFixedHeight(QWIDGETSIZE_MAX);
        }
    }
    _basePanelDockWidget->setPanelState(BasePanelDockWidget::MoveVisible);

    _basePanelDockWidget->setAutoHide(ui->checkBoxAutoHide->isChecked());

    _basePanelWidget->setTabPosition(this->tabPos());
    _basePanelWidget->updateTabs();

    _basePanelWidget->getTabWidget()->setMovable(ui->checkBoxTabsMovable->isChecked());

    QMapIterator<QString, QCheckBox*>i(_allTabs);
    while(i.hasNext()) {
        i.next();
        _basePanelWidget->setVisibleTab(i.key(), i.value()->isChecked());
    }
    this->saveLastState();
}

void SettingsBasePanel::cancelSettings()
{
    QMapIterator<QString, QVariant>i(_lastState);
    while(i.hasNext()) {
        i.next();

        // загружаем состояния QCheckBox-ов
        if(i.key() == tr(TITLE_SHOW_PANEL))
            ui->checkBoxShowPanel->setChecked(i.value().toBool());
        if(i.key() == tr(TITLE_FIXED_SIZE))
            ui->checkBoxFixedSize->setChecked(i.value().toBool());
        if(i.key() == tr(TITLE_AUTO_HIDE))
            ui->checkBoxAutoHide->setChecked(i.value().toBool());

        if(_allTabs.contains(i.key()))
            _allTabs.value(i.key())->setChecked(i.value().toBool());

        // загружаем положение вкладок
        if(i.key() == ui->comboBoxTabsPosition->objectName())
            ui->comboBoxTabsPosition->setCurrentIndex(i.value().toInt());

        // загружаем возможность перемещения вкладок
        if(i.key() == tr(TITLE_TABS_MOVABLE))
            ui->checkBoxTabsMovable->setChecked(i.value().toBool());
    }
}

QMap<QString, QVariant> SettingsBasePanel::settingsList()
{
    QMap<QString, QVariant> buffList;

    // сохраняем состояния QCheckBox-ов
    buffList.insert(ui->checkBoxAutoHide->text(), ui->checkBoxAutoHide->isChecked());
    buffList.insert(ui->checkBoxFixedSize->text(), ui->checkBoxFixedSize->isChecked());
    buffList.insert(ui->checkBoxShowPanel->text(), ui->checkBoxShowPanel->isChecked());

    QMapIterator<QString, QCheckBox*>i(_allTabs);
    while(i.hasNext()) {
        i.next();
        buffList.insert(i.key(), i.value()->isChecked());
    }

    // сохраняем положение вкладок
    buffList.insert("lastTabPos", _tabsPos);
    buffList.insert(ui->comboBoxTabsPosition->objectName(), ui->comboBoxTabsPosition->currentIndex());

    // сохраняем возможность перемещения вкладок
    buffList.insert(ui->checkBoxTabsMovable->text(), ui->checkBoxTabsMovable->isChecked());

    // сохраняем порядок вкладок
    buffList.insert("tabsNumber", this->_basePanelWidget->getTabWidget()->count());
    for(int i = 0; i < this->_basePanelWidget->getTabWidget()->count(); i++) {
        QString tabTitle = this->_basePanelWidget->getTabWidget()->tabToolTip(i);
        if(tabTitle.isEmpty())
            tabTitle = this->_basePanelWidget->getTabWidget()->tabText(i);

        buffList.insert(QString("tab_%1").arg(i), tabTitle);
    }

    // сохраняем активную вкладку
    QString tabTitle = this->_basePanelWidget->getTabWidget()->tabToolTip(this->_basePanelWidget->getTabWidget()->currentIndex());
    if(tabTitle.isEmpty())
        tabTitle = this->_basePanelWidget->getTabWidget()->tabText(this->_basePanelWidget->getTabWidget()->currentIndex());

    buffList.insert("activeTab", tabTitle);

    // сохраняем размер
    buffList.insert("size", _size);

    return buffList;
}

void SettingsBasePanel::setSettingsList (const QMap<QString, QVariant> &settingsList)
{
    if(!settingsList.isEmpty()) {
        int lastTabPos = 0;
        QMapIterator<QString, QVariant>i(settingsList);
        while(i.hasNext()) {
            i.next();

            // загружаем состояния QCheckBox-ов
            if(i.key() == TITLE_SHOW_PANEL)
                ui->checkBoxShowPanel->setChecked(i.value().toBool());
            if(i.key() == TITLE_FIXED_SIZE)
                ui->checkBoxFixedSize->setChecked(i.value().toBool());
            if(i.key() == TITLE_AUTO_HIDE)
                ui->checkBoxAutoHide->setChecked(i.value().toBool());

            if(_allTabs.contains(i.key()))
                _allTabs.value(i.key())->setChecked(i.value().toBool());

            // загружаем положение вкладок
            if(i.key() == ui->comboBoxTabsPosition->objectName())
                ui->comboBoxTabsPosition->setCurrentIndex(i.value().toInt());
            if(i.key() == "lastTabPos")
                lastTabPos = i.value().toInt();

            // загружаем возможность перемещения вкладок
            if(i.key() == TITLE_TABS_MOVABLE)
                ui->checkBoxTabsMovable->setChecked(i.value().toBool());
        }

        // проверяем положение вкладок
        if(ui->comboBoxTabsPosition->currentIndex() != _basePanelWidget->tabPosition()) {
            if(lastTabPos != _basePanelWidget->tabPosition())
                ui->comboBoxTabsPosition->setCurrentIndex(_basePanelWidget->tabPosition());
        }

        // выставляем размер
        if(settingsList.contains("size"))
            _size = settingsList.value("size").toInt();
        if(_size <= 0)
            _size = 200;

        if(_basePanelWidget->panelType() == ISystemGuiCoreBasePanel::BasePanelLeft
           || _basePanelWidget->panelType() == ISystemGuiCoreBasePanel::BasePanelRight)
            _basePanelDockWidget->setFixedWidth(_size);
        else
            _basePanelDockWidget->setFixedHeight(_size);
    }
    this->saveLastState();
    this->applySettings();

    // формируем порядок вкладок
    int tabsNum = 0;
    if(settingsList.contains("tabsNumber"))
        tabsNum = settingsList.value("tabsNumber").toInt();

    QString tabName;
    QVector<QString> tabsName;
    for(int i = 0; i < tabsNum; i++) {
        if(settingsList.contains(QString("tab_%1").arg(i)))
            tabName = settingsList.value(QString("tab_%1").arg(i)).toString();

        if(!tabName.isEmpty())
            tabsName.push_back(tabName);
    }
    this->_basePanelWidget->setTabsPosition(&tabsName);
    tabsName.clear();

    // выставляем активную вкладку
    if(settingsList.contains("activeTab")) {
        QString activeTab = settingsList.value("activeTab").toString();
        this->_basePanelWidget->setActiveTab(activeTab);
    }
}

QIcon SettingsBasePanel::rotateIcon(QIcon icon, int angle)
{
    if(icon.isNull())
        return QIcon();

    // конвертим и разворачиваем иконку
    QPixmap tempPixMap = icon.pixmap(16, 16);
    QMatrix mat;
    mat.rotate(angle%360); //rotation operation
    QPixmap endRotatePixmap = tempPixMap.transformed(mat); //saving the changed QPixmap in a new QPixmap
    QIcon newTabIcon(endRotatePixmap);

    return newTabIcon;
}

void SettingsBasePanel::saveLastState()
{
    _lastState.insert(ui->checkBoxAutoHide->text(), ui->checkBoxAutoHide->isChecked());
    _lastState.insert(ui->checkBoxFixedSize->text(), ui->checkBoxFixedSize->isChecked());
    _lastState.insert(ui->checkBoxShowPanel->text(), ui->checkBoxShowPanel->isChecked());
    _lastState.insert(ui->checkBoxTabsMovable->text(), ui->checkBoxTabsMovable->isChecked());
    _lastState.insert(ui->comboBoxTabsPosition->objectName(), ui->comboBoxTabsPosition->currentIndex());

    QMapIterator<QString, QCheckBox*>i(_allTabs);
    while(i.hasNext()) {
        i.next();
        _lastState.insert(i.key(), i.value()->isChecked());
    }
}

bool SettingsBasePanel::isCompletePrivate() const
{   
    if(_lastState.value(ui->checkBoxAutoHide->text()).toBool() != ui->checkBoxAutoHide->isChecked()
       || _lastState.value(ui->checkBoxFixedSize->text()).toBool() != ui->checkBoxFixedSize->isChecked()
       || _lastState.value(ui->checkBoxShowPanel->text()).toBool() != ui->checkBoxShowPanel->isChecked()
       || _lastState.value(ui->checkBoxTabsMovable->text()).toBool() != ui->checkBoxTabsMovable->isChecked()
       || _lastState.value(ui->comboBoxTabsPosition->objectName()).toInt() != ui->comboBoxTabsPosition->currentIndex())
        return true;

    QMapIterator<QString, QCheckBox*>i(_allTabs);
    while(i.hasNext()) {
        i.next();
        if(_lastState.value(i.key()).toBool() != i.value()->isChecked())
            return true;
    }
    return false;
}

ISystemGuiCoreBasePanel::BasePanelTabPosition SettingsBasePanel::tabPos()
{
    ISystemGuiCoreBasePanel::BasePanelTabPosition tabPos = ISystemGuiCoreBasePanel::BasePanelTabDefault;
    switch(ui->comboBoxTabsPosition->currentIndex()) {
        case 0:
            tabPos = ISystemGuiCoreBasePanel::BasePanelTabDefault;
            break;
        case 1:
            tabPos = ISystemGuiCoreBasePanel::BasePanelTabNorth;
            break;
        case 2:
            tabPos = ISystemGuiCoreBasePanel::BasePanelTabSouth;
            break;
        case 3:
            tabPos = ISystemGuiCoreBasePanel::BasePanelTabWest;
            break;
        case 4:
            tabPos = ISystemGuiCoreBasePanel::BasePanelTabEast;
            break;
        default:
            break;

    }
    return tabPos;
}

void SettingsBasePanel::onCheck(bool isChecked)
{
    QCheckBox* buffCheck = qobject_cast<QCheckBox*>(sender());
    if(buffCheck) {
        if(buffCheck->text() == tr(TITLE_SHOW_PANEL)) {
            ui->checkBoxAutoHide->setEnabled(isChecked);
            ui->checkBoxFixedSize->setEnabled(isChecked);
            ui->checkBoxTabsMovable->setEnabled(isChecked);
            ui->comboBoxTabsPosition->setEnabled(isChecked);
            ui->groupBox->setEnabled(isChecked);
        }
        if(buffCheck->text() == tr(TITLE_AUTO_HIDE)
           && _basePanelDockWidget->panelState() == BasePanelDockWidget::MoveVisible
           && isChecked) {
            // запоминаем последний размер
            if(_basePanelWidget->panelType() == ISystemGuiCoreBasePanel::BasePanelLeft
               || _basePanelWidget->panelType() == ISystemGuiCoreBasePanel::BasePanelRight)
                _size = _basePanelDockWidget->width();
            else
                _size = _basePanelDockWidget->height();
        }
    }
    emit this->settingsChanged();
}

void SettingsBasePanel::onTapsPosChanged(const QString &text)
{
    Q_UNUSED(text)
    emit this->settingsChanged();
}
