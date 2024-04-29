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
    : QWidget { parent }
    , ui { new Ui::PlanningWidget }
{
    ui->setupUi (this);
    createToolBar ();
    setWindowTitle (tr ("Planning"));
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

void PlanningWidget::slotUpdated (const QString &/*guid*/, int /*type*/)
{
    if (!_model) {
        qCritical () << "[MapWidget][slotUpdated] Empty QStandardItemModel object";
        return;
    }
    _model->removeRows (0, _model->rowCount ());
    _model->setRowCount (0);

//    for (auto const &request : requests)
//    {
//        auto const Maps = request->events<MapEvent> ();
//        if (Maps.empty ())
//            continue;

//        for (auto const &cmd : Maps)
//        {
//            QList<QStandardItem*> items_list;

//            //-- Columns::RequestNumber
//            auto item = new QStandardItem (request->number ());
//            item->setIcon (ResourceManager::urgencyCategory (cmd->_urgency_category));
//            item->setData (request->guid (), Qt::UserRole + 1); //-- идентификатор заявки
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::RsRnNumber
//            item = new QStandardItem (request->rsrnNumber ());
//            item->setData (request->guid (), Qt::UserRole + 1); //-- идентификатор заявки
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::Status,
//            item = createStateItem (request);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::MapNumber
//            item = new QStandardItem (QString ("%1 %2 %3").arg (cmd->_cn + 1).arg (tr ("of")).arg (cmd->_cc));
//            item->setData (request->guid (), Qt::UserRole + 1); //-- идентификатор заявки
//            item->setData (cmd->_cn, Qt::UserRole + 2);           //-- номер команды в заявке
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::ReceiverCallsign
//            item = new QStandardItem (cmd->_callsign_receiver);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::Production
//            item = new QStandardItem (EventFunc::productionTypeString (cmd));
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::CommunicationType,  //-- Вид связи
//            item = new QStandardItem  (EventFunc::communicationTypeString (cmd));
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::InterfaceType,      //-- Тип стыка
//            item = new QStandardItem (EventFunc::interfaceTypeString (cmd));
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::Mode,               //-- Режим работы
//            item = new QStandardItem (EventFunc::modeString (cmd));
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::StartDate,          //-- Дата начала
//            item =  new QStandardItem (request->startTime () == 0 ? tr ("ASAP") : QDateTime::fromMSecsSinceEpoch
//                                                                    (request->startTime ()).toString ("dd.MM.yyyy hh:mm:ss")/*.replace (" ", "\n")*/);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::FreqsRx
//            QString freqs_rx;
//            for (auto const &f : cmd->_freqs_rx) {
//                freqs_rx .append (QString::number (f));
//                freqs_rx.append (", ");
//            }
//            if (!freqs_rx.isEmpty ())
//                freqs_rx.remove (freqs_rx.size () - 2, 2);
//            item = new QStandardItem (freqs_rx);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::FreqsTx
//            QString freqs_tx;
//            for (auto const &f : cmd->_freqs_tx) {
//                freqs_tx .append (QString::number (f));
//                freqs_tx.append (", ");
//            }
//            if (!freqs_tx.isEmpty ())
//                freqs_tx.remove (freqs_tx.size () - 2, 2);
//            item = new QStandardItem (freqs_tx);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::ReceiveDate,        //-- Время поступления
//            item = new QStandardItem (QDateTime::fromMSecsSinceEpoch
//                                      (request->receiveTime ()).toString ("dd.MM.yyyy hh:mm:ss"));
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::Operator,           //-- Оператор
//            item = new QStandardItem ();
//            item->setTextAlignment (Qt::AlignCenter);
//            item->setText (cmd->_user);
//            items_list << item;

//            _model->appendRow (items_list);
//        }
//    }

    ui->tableView_->resizeColumnsToContents ();
    ui->tableView_->resizeRowsToContents ();

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

//    auto const requests = _rqm->requests ();
//    for (auto const &request : requests)
//    {
//        auto const Maps = request->events<MapEvent> ();
//        if (Maps.empty ())
//            continue;

//        for (auto const &cmd : Maps)
//        {
//            QList<QStandardItem*> items_list;

//            //-- Columns::RequestNumber
//            auto item = new QStandardItem (request->number ());
//            item->setIcon (ResourceManager::urgencyCategory (cmd->_urgency_category));
//            item->setData (request->guid (), Qt::UserRole + 1); //-- идентификатор заявки
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            item = new QStandardItem (request->rsrnNumber ());
//            item->setData (request->guid (), Qt::UserRole + 1); //-- идентификатор заявки
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::Status,             //-- Статус
//            item = createStateItem (request);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::MapNumber
//            item = new QStandardItem (QString ("%1 %2 %3").arg (cmd->_cn + 1).arg (tr ("of")).arg (cmd->_cc));
//            item->setData (request->guid (), Qt::UserRole + 1); //-- идентификатор заявки
//            item->setData (cmd->_cn, Qt::UserRole + 2);           //-- номер команды в заявке
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::ReceiverCallsign
//            item = new QStandardItem (cmd->_callsign_receiver);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::Production
//            item = new QStandardItem (EventFunc::productionTypeString (cmd));
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::CommunicationType,  //-- Вид связи
//            item = new QStandardItem  (EventFunc::communicationTypeString (cmd));
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::InterfaceType,      //-- Тип стыка
//            item = new QStandardItem (EventFunc::interfaceTypeString (cmd));
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::Mode,               //-- Режим работы
//            item = new QStandardItem (EventFunc::modeString (cmd));
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::StartDate,          //-- Дата начала
//            item =  new QStandardItem (request->startTime () == 0 ? tr ("ASAP") : QDateTime::fromMSecsSinceEpoch
//                                                                    (request->startTime ()).toString ("dd.MM.yyyy hh:mm:ss"));
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::FreqsRx
//            QString freqs_rx;
//            for (auto const &f : cmd->_freqs_rx) {
//                freqs_rx .append (QString::number (f));
//                freqs_rx.append (", ");
//            }
//            if (!freqs_rx.isEmpty ())
//                freqs_rx.remove (freqs_rx.size () - 2, 2);
//            item = new QStandardItem (freqs_rx);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::FreqsTx
//            QString freqs_tx;
//            for (auto const &f : cmd->_freqs_tx) {
//                freqs_tx .append (QString::number (f));
//                freqs_tx.append (", ");
//            }
//            if (!freqs_tx.isEmpty ())
//                freqs_tx.remove (freqs_tx.size () - 2, 2);
//            item = new QStandardItem (freqs_tx);
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::ReceiveDate,        //-- Время поступления
//            item = new QStandardItem (QDateTime::fromMSecsSinceEpoch
//                                      (request->receiveTime ()).toString ("dd.MM.yyyy hh:mm:ss"));
//            item->setTextAlignment (Qt::AlignCenter);
//            items_list << item;

//            //-- Columns::Operator,           //-- Оператор
//            item = new QStandardItem ();
//            item->setTextAlignment (Qt::AlignCenter);
//            item->setText (cmd->_user);
//            items_list << item;

//            _model->appendRow (items_list);
//        }
//    }

    connect (ui->tableView_->selectionModel (), &QItemSelectionModel::selectionChanged,
             this, &PlanningWidget::slotSelectionChanged);

    ui->tableView_->resizeRowsToContents ();

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
//        if (_sfmodel->index (i - 1, Columns::MapNumber).data (Qt::UserRole + 1).toString ()
//                ==
//                _sfmodel->index (i, Columns::MapNumber).data (Qt::UserRole + 1).toString ())
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

//QStandardItem *MapWidget::createStateItem (/*IRequest::ptr_t request*/)
//{
//    if (!request)
//        return new QStandardItem ();

//    auto item = new QStandardItem ();
//    item->setData (request->guid (), Qt::UserRole + 1);

//    switch (request->state ()) {
//    case IRequest::State::RS_1: {
//        item->setText (tr ("Created"));
//    }
//        break;
//    case IRequest::State::RS_2: {
//        item->setText (tr ("Informed"));
//    }
//        break;
//    case IRequest::State::RS_3: {
//        item->setText (tr ("Processed"));
//        item->setBackground (QBrush (GlobalStyleScheet::R_R3_processed));
//    }
//        break;
//    case IRequest::State::RS_4: {
//        item->setText (tr ("Working"));
//        item->setBackground (QBrush (GlobalStyleScheet::R_R4));
//    }
//        break;
//    case IRequest::State::RS_5: {
//        item->setText (tr ("Finishing"));
//        item->setBackground (QBrush (GlobalStyleScheet::R_R5_confirmed));
//    }
//        break;
//    case IRequest::State::RS_6: {
//        item->setText (tr ("Finished"));
//        item->setBackground (QBrush (GlobalStyleScheet::R_R6_canceled));
//    }
//        break;
//    case IRequest::State::RS_7: {
//        item->setText (tr ("Canceled"));
//        item->setBackground (QBrush (GlobalStyleScheet::R_R6_cancel_confirm));
//    }
//    case IRequest::State::RS_8: {
//        item->setText (tr ("Error: waiting new Map"));
//        item->setBackground (QBrush (GlobalStyleScheet::R_R8));
//    }
//        break;
//    case IRequest::State::RS_98: {
//        item->setText (tr ("Timeout"));
//        item->setBackground (QBrush (GlobalStyleScheet::R_R98));
//    }
//        break;
//    case IRequest::State::RS_99: {
//        item->setText (tr ("Error"));
//        item->setBackground (QBrush (GlobalStyleScheet::R_R99));
//    }
//        break;
//    default:
//        break;
//    }

//    item->setEditable (false);
//    return item;
//}



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
