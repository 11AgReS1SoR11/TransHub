#include "PlanningWidget.h"
#include "Object/ResourceManager.h"
#include "ui_PlanningWidget.h"

#include "Object/Settings.h"
#include <ComponentsCore5alpha/ComponentManager>
using namespace ComponentsCore;


#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMenu>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtGui/QPixmap>

PlanningWidget::PlanningWidget (QWidget *parent)
    : ISystemGuiCoreStatusBarTabWindow { parent }
    , ui { new Ui::PlanningWidget }
{
    ui->setupUi (this);
    createToolBar ();
    setWindowTitle (tr ("Planning"));

    connect (Planning::PlanningManager::instance(), &Planning::PlanningManager::aboutAddedObject,
             this, &PlanningWidget::slotUpdated);

    connect (Planning::PlanningManager::instance(), &Planning::PlanningManager::aboutRemovedObject,
             this, &PlanningWidget::slotUpdated);

    connect(Planning::PlanningManager::instance(), &Planning::PlanningManager::aboutToRemoveAllObjects,
             this, &PlanningWidget::clearAll);

    slotInit();
}

PlanningWidget::~PlanningWidget ()
{
    if (_model) {
        delete _model;
        _model = nullptr;
    }
    if (_sfmodel) {
        delete _sfmodel;
        _sfmodel = nullptr;
    }

    delete ui;
}


QPixmap PlanningWidget::GetWindowIcon()
{
    return QPixmap( ":/icons/icons/planning.png" );
}

void PlanningWidget::slotUpdated ()
{
    if (!_model) {
        qCritical () << "[MapWidget][slotUpdated] Empty QStandardItemModel object";
        return;
    }
    _model->removeRows (0, _model->rowCount ());
    _model->setRowCount (0);

    for(auto object : Planning::PlanningManager::instance()->allObjects()) {

        QString name;
        QPair<double, double> NE;


        if (qobject_cast<Planning::Storage*>(object) != nullptr)
        {

            NE = qMakePair<double, double>(qobject_cast<Planning::Storage*>(object)->NE.first,
                                    qobject_cast<Planning::Storage*>(object)->NE.second);
            name = "storage";
            qInfo ().noquote () << QString ("[PlanningManager] Storage loaded successfully");
        }
        else if (qobject_cast<Planning::User*>(object) != nullptr)
        {

            NE = qMakePair<double, double>(qobject_cast<Planning::User*>(object)->NE.first,
                                    qobject_cast<Planning::User*>(object)->NE.second);
            name = "user";
            qInfo ().noquote () << QString ("[PlanningManager] User loaded successfully");
        }
        else if (qobject_cast<Planning::Truck*>(object) != nullptr)
        {

            NE = qMakePair<double, double>(qobject_cast<Planning::Truck*>(object)->NE.first,
                                    qobject_cast<Planning::Truck*>(object)->NE.second);
            name = "truck";
            qInfo ().noquote () << QString ("[PlanningManager] Truck loaded successfully");
        }
        else
        {
            qCritical () << QString ("[PlanningManager] Object load failed");
            continue;
        }

        QList<QStandardItem*> items_list;

        auto item = new QStandardItem (name);
        item->setIcon(QIcon (":/" + name + ".png"));
        item->setData (name, Qt::UserRole + 1);
        item->setTextAlignment (Qt::AlignCenter);
        items_list << item;

        item = new QStandardItem (QString::number(NE.first));
        item->setData (QString::number(NE.first), Qt::UserRole + 1);
        item->setTextAlignment (Qt::AlignCenter);
        items_list << item;

        item = new QStandardItem (QString::number(NE.second));
        item->setData (QString::number(NE.second), Qt::UserRole + 1);
        item->setTextAlignment (Qt::AlignCenter);
        items_list << item;

        _model->appendRow (items_list);

    }

    ui->tableView_->resizeColumnsToContents ();
    //ui->tableView_->resizeRowsToContents ();

    QTimer::singleShot (0, this, &PlanningWidget::updateSpan);
}

void PlanningWidget::slotUpdateFailed (const QString &/*msg*/, int /*type*/)
{
    //showMessage (tr ("Data access error."), MessageWidget::Error);
}

void PlanningWidget::slotInit ()
{

    _model = new QStandardItemModel(0, Columns::ColumnsCount);
    _model->setHorizontalHeaderItem (Columns::Supplier, new QStandardItem (tr ("Supplier")));            //-- Отправитель
    _model->setHorizontalHeaderItem (Columns::Stockroom, new QStandardItem (tr ("Stockroom")));          //-- Склад
    _model->setHorizontalHeaderItem (Columns::Receiver, new QStandardItem (tr ("Receiver")));            //--Получатель


    _sfmodel = new QSortFilterProxyModel ();
    _sfmodel->setSourceModel (_model);

    ui->tableView_->setModel (_sfmodel);
    ui->tableView_->setEditTriggers (QAbstractItemView::NoEditTriggers);
    ui->tableView_->setSelectionMode (QAbstractItemView::SingleSelection);
    ui->tableView_->setSelectionBehavior (QAbstractItemView::SelectRows);
    ui->tableView_->setWordWrap (true);

    ui->tableView_->verticalHeader ()->hide ();
    ui->tableView_->horizontalHeader ()->setMinimumHeight (64);
    ui->tableView_->horizontalHeader()->setMinimumWidth(300);
    ui->tableView_->horizontalHeader ()->setStretchLastSection (true);

    ui->tableView_->setContextMenuPolicy (Qt::CustomContextMenu);
    connect (ui->tableView_, &QTableView::customContextMenuRequested,
             this, &PlanningWidget::slotRequestContextMenu);

    Planning::PlanningManager::instance()->init();

    for(auto object : Planning::PlanningManager::instance()->allObjects()) {

        QString name;
        QPair<double, double> NE;


        if (qobject_cast<Planning::Storage*>(object) != nullptr)
        {

            NE = qMakePair<double, double>(qobject_cast<Planning::Storage*>(object)->NE.first,
                                    qobject_cast<Planning::Storage*>(object)->NE.second);
            name = "storage";
            qInfo ().noquote () << QString ("[PlanningManager] Storage loaded successfully");
        }
        else if (qobject_cast<Planning::User*>(object) != nullptr)
        {

            NE = qMakePair<double, double>(qobject_cast<Planning::User*>(object)->NE.first,
                                    qobject_cast<Planning::User*>(object)->NE.second);
            name = "user";
            qInfo ().noquote () << QString ("[PlanningManager] User loaded successfully");
        }
        else if (qobject_cast<Planning::Truck*>(object) != nullptr)
        {

            NE = qMakePair<double, double>(qobject_cast<Planning::Truck*>(object)->NE.first,
                                    qobject_cast<Planning::Truck*>(object)->NE.second);
            name = "truck";
            qInfo ().noquote () << QString ("[PlanningManager] Truck loaded successfully");
        }
        else
        {
            qCritical () << QString ("[PlanningManager] Object load failed");
            continue;
        }

        QList<QStandardItem*> items_list;

        auto item = new QStandardItem (name);
        item->setIcon(QIcon (":/" + name + ".png"));
        item->setData (name, Qt::UserRole + 1);
        item->setTextAlignment (Qt::AlignCenter);
        items_list << item;

        item = new QStandardItem (QString::number(NE.first));
        item->setData (QString::number(NE.first), Qt::UserRole + 1);
        item->setTextAlignment (Qt::AlignCenter);
        items_list << item;

        item = new QStandardItem (QString::number(NE.second));
        item->setData (QString::number(NE.second), Qt::UserRole + 1);
        item->setTextAlignment (Qt::AlignCenter);
        items_list << item;

        _model->appendRow (items_list);

    }

    connect (ui->tableView_->selectionModel (), &QItemSelectionModel::selectionChanged,
             this, &PlanningWidget::slotSelectionChanged);

    //ui->tableView_->resizeRowsToContents ();

    QTimer::singleShot (0, this, &PlanningWidget::updateSpan);
}

void PlanningWidget::slotRequestContextMenu (const QPoint &/*pos*/)
{
}

void PlanningWidget::updateSpan ()
{
//    ui->tableView_->clearSpans ();
//    for (int i = 1; i < _sfmodel->rowCount (); ++i)
//    {
//        if (_sfmodel->index (i - 1, Columns::Supplier).data (Qt::UserRole + 1).toString ()
//                ==
//                _sfmodel->index (i, Columns::Supplier).data (Qt::UserRole + 1).toString ())
//        {
//            ui->tableView_->setSpan (i - 1, 0, 2, 1);
//        }
//    }
}

void PlanningWidget::updateToolbar ()
{
    qCritical () << "[MapWidget][updateToolbar] UNDER CONSTRUCTION Do it!";
}

void PlanningWidget::slotShowClicked ()
{
}

void PlanningWidget::slotSelectionChanged (const QItemSelection &se, const QItemSelection &/*de*/)
{
    auto const selection = _sfmodel->mapSelectionToSource (se);
    if (!selection.indexes ().isEmpty ())
        _currentIndex = selection.indexes ().at (0);
    else
        _currentIndex = QModelIndex ();
}

void PlanningWidget::OnTabClicked(bool is_only_open)
{
    if ( !is_only_open && isVisible() )
    {
        if ( QWidget * parWidget = qobject_cast<QWidget *>( parent() ) )
            if ( CustomMdiSubWindow * parMdi = dynamic_cast<CustomMdiSubWindow *>( parWidget ) )
                parMdi->TabHide();
    }
    else
    {
        if ( !isVisible() )
            emit OpenTabWindow();
    }
}

void PlanningWidget::clearAll()
{
    if (!_model) {
        qCritical () << "[MapWidget][slotUpdated] Empty QStandardItemModel object";
        return;
    }
    _model->removeRows (0, _model->rowCount ());
    _model->setRowCount (0);
}

void PlanningWidget::createToolBar ()
{
    _toolBar = new QToolBar (this);
    _toolBar->setIconSize (QSize (32, 32));
    _toolBar->setOrientation (Qt::Vertical);
    _toolBar->setToolButtonStyle (Qt::ToolButtonTextUnderIcon);

    _toolBar->addAction (QIcon (":/icons/show_32x32.png"), tr ("Show"), this, &PlanningWidget::slotShowClicked);
    _toolBar->addSeparator ();
    _toolBar->addAction (QIcon (":/icons/edit_32x32.png"), tr ("Edit"));
    _toolBar->addAction (QIcon (":/icons/cancel_32x32.png"), tr ("Cancel"));
    _toolBar->addSeparator ();
    _toolBar->addAction (QIcon (":/icons/print_32x32.png"), tr ("Print\njournal"));
    _toolBar->addSeparator ();
    _toolBar->addAction (QIcon (":/icons/export_32x32.png"), tr ("Export"));
    _toolBar->addAction (QIcon (":/icons/import_32x32.png"), tr ("Import"));
    _toolBar->addSeparator ();
    _toolBar->addAction (QIcon (":/icons/help_32x32.png"), tr ("Help"));

    ui->mainLayout_->insertWidget (0, _toolBar);
}

//void MapWidget::showMessage (const QString &msg, MessageWidget::MessageType type, bool vis)
//{
//    if (!_msgWidget) {
//        //-- Виджет с окном сообщения
//        _msgWidget = new MessageWidget (this);
//        _msgWidget->setWordWrap (true);
//        _msgWidget->hide ();
//        ui->mainLayout_->addWidget (_msgWidget);
//    }

//    _msgWidget->setText (msg);
//    _msgWidget->setMessageType (type);
//    _msgWidget->setCloseButtonVisible (vis);
//    if (!_msgWidget->isVisible ())
//        _msgWidget->animatedShow ();
//}
