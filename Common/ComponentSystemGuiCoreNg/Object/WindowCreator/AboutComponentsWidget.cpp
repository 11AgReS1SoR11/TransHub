#include "AboutComponentsWidget.h"
#include "ui_AboutComponentsWidget.h"

#include <ComponentsCore5alpha/ComponentManager>
using namespace ComponentsCore;

//#include "../StatusBarInfo/SystemWordWraper.h"
#include <QtCore/QDebug>

//#define PLUGIN_NOT_LOADED   0xDC143C
//#define PLUGIN_LOADED       0x177245
//#define PLUGIN_WARNING      0xEA7500

#define TABLE_ROW_COLOR     0xd5d5d5

AboutComponentsWidget::AboutComponentsWidget (QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::AboutComponentsWidget)
{
    ui->setupUi (this);
    //setWindowTitle (tr (ACTION_COMPONENTS_STATUS));

    ui->_labelFindIcon->setPixmap (QPixmap (":/icons/icons/magnifier.png"));
    ui->_editFindText->setToolTip (tr ("Find by name"));

    init ();

    connect (ui->_componentsTree, &QTreeWidget::currentItemChanged,
             this, &AboutComponentsWidget::changeCurrentItem);
    connect (ui->_editFindText, &QLineEdit::textChanged,
             this, &AboutComponentsWidget::findComponent);
}

AboutComponentsWidget::~AboutComponentsWidget ()
{
    _lastSelected = nullptr;
    delete ui;
}

void AboutComponentsWidget::init ()
{
    unsigned int resolvedComponents {0};
    unsigned int loadedComponents {0};
    unsigned int initializedComponents {0};
    unsigned int runningComponents {0};
    unsigned int stoppedComponents {0};

    auto const specs = ComponentManager::components ();

    //--- Ищем плагины с состоянием Resolved
    {
        int row {0};
        QTreeWidgetItem* resolvedItem = new QTreeWidgetItem ();
        resolvedItem->setIcon (0, QIcon (":/icons/icons/component2Warning.png"));
        resolvedItem->setText (0, tr ("Resolved"));
        for (auto const spec : specs)
        {
            auto v = spec->dependencies();

            if (spec->state () == ComponentSpec::Resolved && spec->component () != nullptr)
            {
                resolvedComponents++;
                QIcon icon = QIcon (":/icons/icons/component2Warning.png");

                QTreeWidgetItem* item = new QTreeWidgetItem (resolvedItem);
                item->setIcon (0, icon);
                item->setText (0, spec->name ());
                item->setData (0, Qt::UserRole + 1, spec->name ());

                if (row % 2 == 0)
                {
                    QLinearGradient linearGrad (QPointF (0, 0), QPointF (200, 0));
                    linearGrad.setColorAt (0, QColor (TABLE_ROW_COLOR));
                    linearGrad.setColorAt (1, Qt::white);
                    item->setBackground (0, linearGrad);
                }
                row++;
            }
        }
        if (resolvedItem->childCount () != 0)
            ui->_componentsTree->addTopLevelItem (resolvedItem);
        else
            delete resolvedItem;
    }

    ui->_labelResolvedNum->setText (QString::number (resolvedComponents));

    //--- Ищем плагины с состоянием Loaded
    {
        int row {0};
        QTreeWidgetItem* loadedItem = new QTreeWidgetItem ();
        loadedItem->setIcon (0, QIcon (":/icons/icons/component2Warning.png"));
        loadedItem->setText (0, tr ("Loaded"));
        for (auto const spec : specs)
        {
            if (spec->state () == ComponentSpec::Loaded && spec->component () != nullptr)
            {
                loadedComponents++;
                QIcon icon = QIcon (":/icons/icons/component2Warning.png");

                QTreeWidgetItem* item = new QTreeWidgetItem (loadedItem);
                item->setIcon (0, icon);
                item->setText (0, spec->name ());
                item->setData (0, Qt::UserRole + 1, spec->name ());

                if (row % 2 == 0)
                {
                    QLinearGradient linearGrad (QPointF (0, 0), QPointF (200, 0));
                    linearGrad.setColorAt (0, QColor (TABLE_ROW_COLOR));
                    linearGrad.setColorAt (1, Qt::white);
                    item->setBackground (0, linearGrad);
                }
                row++;
            }
        }
        if (loadedItem->childCount () != 0)
            ui->_componentsTree->addTopLevelItem (loadedItem);
        else
            delete loadedItem;
    }

    ui->_labelLoadedNum->setText (QString::number (loadedComponents));

    //--- Ищем плагины с состоянием Initialized
    {
        int row {0};
        QTreeWidgetItem* initializedItem = new QTreeWidgetItem ();
        initializedItem->setIcon (0, QIcon (":/icons/icons/component2Warning.png"));
        initializedItem->setText (0, tr ("Resolved"));
        for (auto const spec : specs)
        {
            if (spec->state () == ComponentSpec::Initialized && spec->component () != nullptr)
            {
                initializedComponents++;
                QIcon icon = QIcon (":/icons/icons/component2Warning.png");

                QTreeWidgetItem* item = new QTreeWidgetItem (initializedItem);
                item->setIcon (0, icon);
                item->setText (0, spec->name ());
                item->setData (0, Qt::UserRole + 1, spec->name ());

                if (row % 2 == 0)
                {
                    QLinearGradient linearGrad (QPointF (0, 0), QPointF (200, 0));
                    linearGrad.setColorAt (0, QColor (TABLE_ROW_COLOR));
                    linearGrad.setColorAt (1, Qt::white);
                    item->setBackground (0, linearGrad);
                }
                row++;
            }
        }
        if (initializedItem->childCount () != 0)
            ui->_componentsTree->addTopLevelItem (initializedItem);
        else
            delete initializedItem;
    }

    ui->_labelInitializedNum->setText (QString::number (initializedComponents));

    //--- Ищем плагины с состоянием Running
    {
        int row = 0;
        QTreeWidgetItem* runningItem = new QTreeWidgetItem ();
        runningItem->setIcon (0, QIcon (":/icons/icons/component2Ok.png"));
        runningItem->setText (0, tr ("Running"));
        for (auto const spec : specs)
        {
            if (spec->state () == ComponentSpec::Running && spec->component () != nullptr)
            {
                runningComponents++;

                //--- перебираем все зависимости и ищем не проинициализированные 'optional' зависимости
                bool notLoadOptionalDepending = false;

                QHash<ComponentDependency, ComponentSpec *> deps = spec->dependencySpecs ();
                QHashIterator<ComponentDependency, ComponentSpec *> iter (deps);
                while (iter.hasNext ()) {
                    iter.next ();
                    if (iter.value ()->state () != ComponentSpec::Running)
                    {
                        notLoadOptionalDepending = true;
                        break;
                    }
                }

                QIcon icon;
                if (notLoadOptionalDepending)
                    icon = QIcon (":/icons/icons/component2Warning.png");
                else
                    icon = QIcon (":/icons/icons/component2Ok.png");

                QTreeWidgetItem *item = new QTreeWidgetItem (runningItem);
                item->setIcon (0, icon);
                item->setText (0, spec->name ());
                item->setData (0, Qt::UserRole + 1, spec->name ());

                if (row % 2 == 0)
                {
                    QLinearGradient linearGrad (QPointF (0, 0), QPointF (200, 0));
                    linearGrad.setColorAt (0, QColor (TABLE_ROW_COLOR));
                    linearGrad.setColorAt (1, Qt::white);
                    item->setBackground (0, linearGrad);
                }
                row++;
            }
        }
        if (runningItem->childCount () != 0)
            ui->_componentsTree->addTopLevelItem (runningItem);
        else
            delete runningItem;
    }

    ui->_labelRunningNum->setText (QString::number (runningComponents));

    //--- Ищем плагины с состоянием Stopped
    {
        int row {0};
        QTreeWidgetItem* stoppedItem = new QTreeWidgetItem ();
        stoppedItem->setIcon (0, QIcon (":/icons/icons/component2Warning.png"));
        stoppedItem->setText (0, tr ("Stopped"));
        for (auto const spec : specs)
        {
            if (spec->state () == ComponentSpec::Stopped && spec->component () != nullptr)
            {
                stoppedComponents++;
                QIcon icon = QIcon (":/icons/icons/component2Warning.png");

                QTreeWidgetItem *item = new QTreeWidgetItem (stoppedItem);
                item->setIcon (0, icon);
                item->setText (0, spec->name ());
                item->setData (0, Qt::UserRole + 1, spec->name ());

                if (row % 2 == 0)
                {
                    QLinearGradient linearGrad (QPointF (0, 0), QPointF (200, 0));
                    linearGrad.setColorAt (0, QColor (TABLE_ROW_COLOR));
                    linearGrad.setColorAt (1, Qt::white);
                    item->setBackground (0, linearGrad);
                }
                row++;
            }
        }
        if (stoppedItem->childCount () != 0)
            ui->_componentsTree->addTopLevelItem (stoppedItem);
        else
            delete stoppedItem;
    }

    ui->_labelStoppedNum->setText (QString::number (stoppedComponents));
}

void AboutComponentsWidget::clean ()
{
    ui->_labelComponentName->clear ();
    ui->_labelComponentStatus->clear ();
    ui->_labelComponentVersion->clear ();
    ui->_labelComponentPath->clear ();

    ui->_editComponentAbout->clear ();
    ui->_editComponentError->clear ();
}

void AboutComponentsWidget::changeCurrentItem (QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    (void)previous;
    clean ();

    if (!current || current->data (0, Qt::UserRole + 1).toString ().isEmpty ())
        return;

    auto const specs = ComponentManager::components ();
    for (auto const spec : specs)
    {
        if (spec->name () == current->data (0, Qt::UserRole + 1).toString ())
        {
            ui->_labelComponentName->setText (spec->name ());
            ui->_labelComponentVersion->setText (spec->version ());

            ComponentSpec::State st = spec->state ();
            if (st == ComponentSpec::Invalid)
                ui->_labelComponentStatus->setText (tr ("Invalid"));
            else if (st == ComponentSpec::Read)
                ui->_labelComponentStatus->setText (tr ("Read"));
            else if (st == ComponentSpec::Resolved)
                ui->_labelComponentStatus->setText (tr ("Resolved"));
            else if (st == ComponentSpec::Loaded)
                ui->_labelComponentStatus->setText (tr ("Loaded"));
            else if (st == ComponentSpec::Initialized)
                ui->_labelComponentStatus->setText (tr ("Initialized"));
            else if (st == ComponentSpec::Running)
                ui->_labelComponentStatus->setText (tr ("Running"));
            else if (st == ComponentSpec::Stopped)
                ui->_labelComponentStatus->setText (tr ("Stopped"));
            else if (st == ComponentSpec::Deleted)
                ui->_labelComponentStatus->setText (tr ("Deleted"));

            ui->_labelComponentPath->setText (spec->filePath ());

            ui->_editComponentAbout->setText (spec->description ());

            auto const list = spec->errors ();
            for (auto const &str : list)
                ui->_editComponentError->append (str);
        }
    }
}

void AboutComponentsWidget::findComponent (const QString &text)
{
    ui->_componentsTree->expandAll ();
    ui->_componentsTree->clearSelection ();

    if (text.isEmpty ()) {
        ui->_componentsTree->scrollToTop ();
    } else {
        QList<QTreeWidgetItem*> findItems = ui->_componentsTree->findItems
                (text, Qt::MatchContains | Qt::MatchRecursive, 0);
        if (findItems.size () != 0)
        {
            QTreeWidgetItem *item = findItems[0];
            ui->_componentsTree->setItemSelected (item, true);
            ui->_componentsTree->scrollToItem (item);
        }
    }
}
