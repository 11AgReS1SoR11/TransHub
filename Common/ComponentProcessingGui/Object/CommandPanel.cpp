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

void CommandPanel::slotUpdated (const QString &/*guid*/, int /*type*/)
{
    if (!_model) {
        qCritical () << "[CommandPanel][slotUpdated] Empty QStandardItemModel object";
        return;
    }

    _model->removeRows (0, _model->rowCount ());
    _model->setRowCount (0);

//    for (auto const &request : _manager->requests ())
//    {
//        auto const commands = request->events<CommandEvent> ();
//        if (commands.empty ())
//            continue;

//        for (auto const &cmd : commands)
//        {
//            QList<QStandardItem*> items_list;

//            //-- RequestNumber
//            auto item = new QStandardItem (request->number ());
//            item->setIcon (ResourceManager::urgencyCategory (cmd->_urgency_category));
//            item->setData (request->guid (), Qt::UserRole + 1);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- RsRnNumber
//            item = new QStandardItem (request->rsrnNumber ());
//            item->setData (request->guid (), Qt::UserRole + 1);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Receiver
//            item = new QStandardItem (cmd->_callsign_receiver);
//            item->setData (request->guid (), Qt::UserRole + 1);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            item = new QStandardItem (QString ("%1 %2 %3").arg (cmd->_cn + 1).arg (tr ("of")).arg (cmd->_cc));
//            item->setData (request->guid (), Qt::UserRole + 1);
//            item->setData (cmd->_cn, Qt::UserRole + 2);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            item =  new QStandardItem (request->startTime () == 0 ? tr ("ASAP") : QDateTime::fromMSecsSinceEpoch
//                                                                    (request->startTime ()).toString ("dd.MM.yyyy hh:mm:ss"));
//            item->setData (request->guid (), Qt::UserRole + 1);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            _model->appendRow (items_list);
//        }
//    }

    ui->tableView_->resizeRowsToContents ();
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


//    for (auto const &request : _manager->requests ())
//    {
//        auto const commands = request->events<CommandEvent> ();
//        if (commands.empty ())
//            continue;

//        for (auto const &v : commands)
//        {
//            QList<QStandardItem*> items_list;

//            auto item = new QStandardItem (request->number ());
//            item->setIcon (ResourceManager::urgencyCategory (v->_urgency_category));
//            item->setData (request->guid (), Qt::UserRole + 1);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            item = new QStandardItem (request->rsrnNumber ());
//            item->setData (request->guid (), Qt::UserRole + 1);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            item = new QStandardItem (v->_callsign_receiver);
//            item->setData (request->guid (), Qt::UserRole + 1);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            item = new QStandardItem (QString ("%1 %2 %3").arg (v->_cn + 1).arg (tr ("of")).arg (v->_cc));
//            item->setData (request->guid (), Qt::UserRole + 1);
//            item->setData (v->_cn, Qt::UserRole + 2);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            item =  new QStandardItem (request->startTime () == 0 ? tr ("ASAP") : QDateTime::fromMSecsSinceEpoch
//                                                                    (request->startTime ()).toString ("dd.MM.yyyy hh:mm:ss"));
//            item->setData (request->guid (), Qt::UserRole + 1);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            _model->appendRow (items_list);
//        }
//    }

    ui->tableView_->resizeRowsToContents ();
    ui->tableView_->horizontalHeader ()->setSectionResizeMode (Columns::Receiver, QHeaderView::Stretch);
}

void CommandPanel::showMessage (const QString &msg, bool vis)
{/*
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
        _msgWidget->animatedShow ();*/
}
