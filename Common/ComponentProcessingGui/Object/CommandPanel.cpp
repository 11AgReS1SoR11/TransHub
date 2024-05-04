#include "CommandPanel.h"
#include "ui_CommandPanel.h"
#include "Object/ResourceManager.h"

#include "Object/Settings.h"
#include <ComponentsCore5alpha/ComponentManager>
using namespace ComponentsCore;


#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMenu>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtGui/QPixmap>


using namespace ComponentsCore;

CommandPanel::CommandPanel (QWidget *parent)
    : QWidget { parent }
    , ui { new Ui::CommandPanel }
{
    ui->setupUi (this);

    connect (Planning::PlanningManager::instance(), &Planning::PlanningManager::aboutAddedObject,
             this, &CommandPanel::slotUpdated);

    connect (Planning::PlanningManager::instance(), &Planning::PlanningManager::aboutRemovedObject,
             this, &CommandPanel::slotUpdated);

    connect(Planning::PlanningManager::instance(), &Planning::PlanningManager::aboutToRemoveAllObjects,
             this, &CommandPanel::clearAll);

    slotInit();
}

CommandPanel::~CommandPanel ()
{
    if (_model) {
        delete _model;
        _model = nullptr;
    }
    if (_sfModel) {
        delete _sfModel;
        _sfModel = nullptr;
    }

    delete ui;
}

void CommandPanel::slotUpdated ()
{
    if (!_model) {
        qCritical () << "[CommandPanel][slotUpdated] Empty QStandardItemModel object";
        return;
    }

    _model->removeRows (0, _model->rowCount ());
    _model->setRowCount (0);

    for(auto object : Planning::PlanningManager::instance()->allObjects()) {

        QString name;
        QPair<double, double> NE;

        if (object && qobject_cast<Planning::Storage*>(object) != nullptr)
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

    //ui->tableView_->resizeRowsToContents ();
    ui->tableView_->resizeColumnsToContents ();
    ui->tableView_->horizontalHeader ()->setSectionResizeMode (Columns::Receiver, QHeaderView::Stretch);
}

void CommandPanel::slotUpdateFailed (const QString &/*msg*/, int /*type*/)
{
}

void CommandPanel::slotInit ()
{
    _model = new QStandardItemModel(0, Columns::ColumnsCount);
    _model->setHorizontalHeaderItem (Columns::Supplier, new QStandardItem (tr ("Supplier")));            //-- Отправитель
    _model->setHorizontalHeaderItem (Columns::Stockroom, new QStandardItem (tr ("Stockroom")));          //-- Склад
    _model->setHorizontalHeaderItem (Columns::Receiver, new QStandardItem (tr ("Receiver")));            //--Получатель


    _sfModel = new QSortFilterProxyModel ();
    _sfModel->setSourceModel (_model);

    ui->tableView_->setModel (_sfModel);
    ui->tableView_->setEditTriggers (QAbstractItemView::NoEditTriggers);
    ui->tableView_->setSelectionMode (QAbstractItemView::SingleSelection);
    ui->tableView_->setSelectionBehavior (QAbstractItemView::SelectRows);
    ui->tableView_->setWordWrap (true);

    ui->tableView_->verticalHeader ()->hide ();
    ui->tableView_->horizontalHeader ()->setMinimumHeight (64);
    ui->tableView_->horizontalHeader ()->setMinimumWidth (ui->tableView_->width() / 3);
    ui->tableView_->horizontalHeader ()->setStretchLastSection (true);

    ui->tableView_->setContextMenuPolicy (Qt::CustomContextMenu);

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

    ui->tableView_->resizeRowsToContents ();
    ui->tableView_->horizontalHeader ()->setSectionResizeMode (Columns::Receiver, QHeaderView::Stretch);
}

void CommandPanel::clearAll()
{
    if (!_model) {
        qCritical () << "[CommandPanel][slotUpdated] Empty QStandardItemModel object";
        return;
    }

    _model->removeRows (0, _model->rowCount ());
    _model->setRowCount (0);
}

void CommandPanel::showMessage (const QString &msg, bool vis)
{
    /*
    if (!_msgWidget) {
        //-- Виджет с окном сообщения
        _msgWidget = new MessageWidget (this);
        _msgWidget->setWordWrap (true);
        _msgWidget->hide ();
        ui->mainLayout_->addWidget (_msgWidget);
    }

    _msgWidget->setText (msg);
    _msgWidget->setMessageType (type);
    _msgWidget->setCloseButtonVisible (vis);
    if (!_msgWidget->isVisible ())
        _msgWidget->animatedShow ();
*/
}
