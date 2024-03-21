#include "BasePanelWidget.h"
#include "ui_BasePanelWidget.h"

#include <QtGui/QMatrix>
#include <QtGui/QPixmap>
#include <QtCore/QDebug>

BasePanelWidget::BasePanelWidget(BasePanelType panelType, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BasePanelWidget)
{
    ui->setupUi(this);

    _panelType = panelType;

    switch (_panelType) {
    case BasePanelLeft:
        _tabPosition = BasePanelTabWest;
        break;
    case BasePanelRight:
        _tabPosition = BasePanelTabEast;
        break;
    case BasePanelBottom:
        _tabPosition = BasePanelTabNorth;
        break;
    default:
        _tabPosition = BasePanelTabWest;
        break;
    }

    ui->tabWidget->setMovable(true);
}

BasePanelWidget::~BasePanelWidget()
{
    delete ui;
}

QTabWidget* BasePanelWidget::getTabWidget()
{
    return ui->tabWidget;
}

void BasePanelWidget::initTabs()
{
    switch(_tabPosition) {
    case BasePanelTabNorth:
        ui->tabWidget->setTabPosition(QTabWidget::North);
        break;
    case BasePanelTabSouth:
        ui->tabWidget->setTabPosition(QTabWidget::South);
        break;
    case BasePanelTabWest:
        ui->tabWidget->setTabPosition(QTabWidget::West);
        break;
    case BasePanelTabEast:
        ui->tabWidget->setTabPosition(QTabWidget::East);
        break;
    default:
        ui->tabWidget->setTabPosition(QTabWidget::West);
        break;
    }

    QMapIterator <QString, QWidget*>i(_allTabs);
    while(i.hasNext()) {
        i.next();

        QIcon tabIcon;
        QString tabText = i.key();
        QWidget* tabWidget = i.value();

        if(_tabPosition == BasePanelTabWest)
            tabIcon = this->rotateIcon(_allTabsIcon.value(i.key()));
        else if(_tabPosition == BasePanelTabEast)
            tabIcon = this->rotateIcon(_allTabsIcon.value(i.key()), 270);
        else
            tabIcon = _allTabsIcon.value(i.key());

        if(tabIcon.isNull()) {
            ui->tabWidget->addTab(tabWidget, tabIcon, tabText);
        } else {
            int tabNum = ui->tabWidget->addTab(tabWidget, tabIcon, QString(""));
            ui->tabWidget->setTabToolTip(tabNum, tabText);
        }
    }
}

void BasePanelWidget::updateTabs()
{
    switch(_tabPosition) {
    case BasePanelTabNorth:
        ui->tabWidget->setTabPosition(QTabWidget::North);
        break;
    case BasePanelTabSouth:
        ui->tabWidget->setTabPosition(QTabWidget::South);
        break;
    case BasePanelTabWest:
        ui->tabWidget->setTabPosition(QTabWidget::West);
        break;
    case BasePanelTabEast:
        ui->tabWidget->setTabPosition(QTabWidget::East);
        break;
    default:
        ui->tabWidget->setTabPosition(QTabWidget::West);
        break;
    }

    for(int i = 0; i < ui->tabWidget->count(); i++) {
        QIcon tabIcon;
        QString tabName = ui->tabWidget->tabToolTip(i);
        if(tabName.isEmpty())
            tabName = ui->tabWidget->tabText(i);

        if(_tabPosition == BasePanelTabWest)
            tabIcon = this->rotateIcon(_allTabsIcon.value(tabName));
        else if(_tabPosition == BasePanelTabEast)
            tabIcon = this->rotateIcon(_allTabsIcon.value(tabName), 270);
        else
            tabIcon = _allTabsIcon.value(tabName);

        if(!tabIcon.isNull())
            ui->tabWidget->setTabIcon(i, tabIcon);
    }
}

void BasePanelWidget::setActiveTab(const QString &tabName)
{
    if(tabName.isEmpty())
        return;

    for(int i = 0; i < ui->tabWidget->count(); i++) {
        QString tabTitle = ui->tabWidget->tabToolTip(i);
        if(tabTitle.isEmpty())
            tabTitle = ui->tabWidget->tabText(i);

        if(tabTitle == tabName) {
            ui->tabWidget->setCurrentIndex(i);
            return;
        }
    }
}

void BasePanelWidget::setTabsPosition(QVector<QString> *tabsName)
{
    if (tabsName == nullptr)
        return;

    QVector<QString> tempOpenedTabs;
    for(int i = 0; i < ui->tabWidget->count(); i++) {
        QString tabTitle = ui->tabWidget->tabToolTip(i);
        if(tabTitle.isEmpty())
            tabTitle = ui->tabWidget->tabText(i);

        tempOpenedTabs.push_back(tabTitle);
    }

    if(tempOpenedTabs.size() == 0)
        return;

    ui->tabWidget->clear();

    // формируем вкладки которые были ранее открыты
    for(int j = 0; j < tabsName->size(); j++) {
        if(tempOpenedTabs.contains(tabsName->operator [](j)))
            this->setVisibleTab(tabsName->operator [](j), true);
    }

    // формируем оставшиеся вкладки
    for(int k = 0; k < tempOpenedTabs.size(); k++) {
        if(tabsName->contains(tempOpenedTabs.operator [](k)) == false)
            this->setVisibleTab(tempOpenedTabs.operator [](k), true);
    }
    tempOpenedTabs.clear();
}

void BasePanelWidget::setVisibleTab(const QString &tabName, bool value)
{
    if(value == false) {
        for(int i = 0; i < ui->tabWidget->count(); i++) {
            QString buffTabName = ui->tabWidget->tabToolTip(i);
            if(buffTabName.isEmpty())
                buffTabName = ui->tabWidget->tabText(i);

            if(buffTabName == tabName) {
                ui->tabWidget->removeTab(i);
                return;
            }
        }
    } else {
        if(_allTabs.contains(tabName) == true) {
            QIcon tabIcon;
            QString tabText = tabName;

            if(this->isVisibleTab(tabName))
                return;

            QWidget* tabWidget = _allTabs.value(tabName);

            if(_tabPosition == BasePanelTabWest)
                tabIcon = this->rotateIcon(_allTabsIcon.value(tabName));
            else if(_tabPosition == BasePanelTabEast)
                tabIcon = this->rotateIcon(_allTabsIcon.value(tabName), 270);
            else
                tabIcon = _allTabsIcon.value(tabName);

            if(tabIcon.isNull()) {
                ui->tabWidget->addTab(tabWidget, tabIcon, tabText);
            } else {
                int tabNum = ui->tabWidget->addTab(tabWidget, tabIcon, QString(""));
                ui->tabWidget->setTabToolTip(tabNum, tabText);
            }
        }
    }
}

QIcon BasePanelWidget::rotateIcon (const QIcon &icon, int angle)
{
    if (icon.isNull ())
        return QIcon ();

    // конвертим и разворачиваем иконку
    QPixmap tempPixMap = icon.pixmap (16, 16);
    QMatrix mat;
    mat.rotate (angle%360); //rotation operation
    QPixmap endRotatePixmap = tempPixMap.transformed (mat); //saving the changed QPixmap in a new QPixmap
    QIcon newTabIcon(endRotatePixmap);

    return newTabIcon;
}

bool BasePanelWidget::isVisibleTab (const QString &tabName)
{
    for (int i = 0; i < ui->tabWidget->count (); i++)
    {
        QString buffTabName = ui->tabWidget->tabToolTip (i);
        if (buffTabName.isEmpty ())
            buffTabName = ui->tabWidget->tabText (i);

        if (buffTabName == tabName)
            return true;
    }
    return false;
}

ISystemGuiCoreBasePanel::BasePanelType BasePanelWidget::panelType()
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    return _panelType;
}

ISystemGuiCoreBasePanel::BasePanelTabPosition BasePanelWidget::tabPosition()
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    return _tabPosition;
}

void BasePanelWidget::setTabPosition(ISystemGuiCoreBasePanel::BasePanelTabPosition tabPosition)
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    if(tabPosition == BasePanelTabDefault) {
        switch (_panelType) {
        case BasePanelLeft:
            _tabPosition = BasePanelTabWest;
            break;
        case BasePanelRight:
            _tabPosition = BasePanelTabEast;
            break;
        case BasePanelBottom:
            _tabPosition = BasePanelTabNorth;
            break;
        default:
            _tabPosition = BasePanelTabWest;
            break;
        }
    } else {
        _tabPosition = tabPosition;
    }
}

void BasePanelWidget::addTabWidget (const QString &tabName,
                                    const QIcon &tabIcon,
                                    QWidget *tabWidget)
{
    QMutexLocker lock(&_mutex);
    Q_UNUSED(lock)

    if(tabWidget == nullptr) {
        qCritical() << QString("[BasePanelWidget][addTabWidget] Add tab failed! Add tab is NULL! (tab name: %1)").arg(tabName);
        return;
    }

    if(tabName.isEmpty()) {
        qCritical() << QString("[BasePanelWidget][addTabWidget] Add tab failed! Add tab name is Empty!");
        return;
    }

    if(_allTabs.contains(tabName)) {
        qCritical() << QString("[BasePanelWidget][addTabWidget] Add tab failed! Add tab exists! (tab name: %1)").arg(tabName);
        return;
    }

    _allTabs.insert(tabName, tabWidget);
    _allTabsIcon.insert(tabName, tabIcon);
}
