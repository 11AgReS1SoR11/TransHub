#include "StatusBarInfoWidgetData.h"
#include "ui_StatusBarInfoWidgetData.h"
#include <QDebug>

StatusBarInfoWidgetData::StatusBarInfoWidgetData (QStandardItemModel *model, QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::StatusBarInfoWidgetData)
{
    ui->setupUi (this);
    this->setWindowTitle (tr ("Список уведомлений"));
    this->setWindowIcon (QIcon (":/icons/icons/information.png"));

    this->ui->tableView->setSelectionMode (QAbstractItemView::SingleSelection);
    this->ui->tableView->setSelectionBehavior (QAbstractItemView::SelectRows);
    this->ui->tableView->setEditTriggers (QAbstractItemView::NoEditTriggers);
    this->ui->tableView->setItemDelegate (new TableColorDelegate(this));
    this->ui->tableView->setContextMenuPolicy (Qt::CustomContextMenu);

    this->_model = model;
    this->_modelRows = model->rowCount ();
    this->_currentIndex = this->_model->index(this->_model->rowCount(), this->_model->columnCount());
    this->_autoScroll = true;

    this->initWindow();

    connect(this->_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(autoScroll()));
    connect(this->ui->tableView, SIGNAL(pressed(QModelIndex)), this, SLOT(clickInCell(QModelIndex)));
    connect(this->ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClickInCell(QModelIndex)));
    connect(this->ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuView(QPoint)));
}

StatusBarInfoWidgetData::~StatusBarInfoWidgetData()
{
    delete ui;
    this->_model = nullptr;
}

void StatusBarInfoWidgetData::initWindow()
{
    QMutexLocker a(&this->_mutex);
    Q_UNUSED(a)

    this->ui->tableView->setModel(this->_model);
    this->ui->tableView->horizontalHeader()->setSectionResizeMode (this->_model->columnCount() - 1, QHeaderView::Stretch);
    this->ui->tableView->hideColumn(0);
    this->ui->tableView->resizeColumnToContents(1);
    this->ui->tableView->resizeColumnToContents(2);
    this->ui->tableView->resizeColumnToContents(3);

    if(this->_autoScroll == true)
        this->ui->tableView->scrollToBottom();
}

void StatusBarInfoWidgetData::contextMenuView(const QPoint &pos)
{
    QModelIndex buffIndex = this->_model->index(this->_currentIndex.row(), this->_currentIndex.column());
    if(buffIndex.isValid())
    {
        QMenu* menu = new QMenu(this);
        QAction* buffAction = menu->addAction(QIcon(":/icons/icons/minus-circle.png"), tr("Удалить из списка"));
        buffAction->setIconVisibleInMenu(true);
        connect(buffAction, SIGNAL(triggered()), this, SLOT(deleteElement()));

        menu->addSeparator();
        buffAction = menu->addAction(QIcon(":/icons/icons/icon_cancel.png"), tr("Очистить список"));
        buffAction->setIconVisibleInMenu(true);
        connect(buffAction, SIGNAL(triggered()), this, SLOT(deleteAll()));

        //menu->addSeparator();
        if(this->_autoScroll == true)
            buffAction = menu->addAction(QIcon(":/icons/icons/fill-270.png"), tr("Проматывать в конец"));
        else
            buffAction = menu->addAction(QIcon(":/icons/icons/fill-270_gray.png"), tr("Проматывать в конец"));

        buffAction->setIconVisibleInMenu(true);
        buffAction->setCheckable(true);
        buffAction->setChecked(this->_autoScroll);
        connect(buffAction, SIGNAL(toggled(bool)), this, SLOT(autoScroll(bool)));

        menu->exec(this->mapToGlobal(pos));

        delete menu;
        menu = nullptr;
    }
    else
    {
        QMenu* menu = new QMenu(this);
        QAction* buffAction = nullptr;
        if(this->_autoScroll == true)
            buffAction = menu->addAction(QIcon(":/icons/icons/fill-270.png"), tr("Проматывать в конец"));
        else
            buffAction = menu->addAction(QIcon(":/icons/icons/fill-270_gray.png"), tr("Проматывать в конец"));
        buffAction->setCheckable(true);
        buffAction->setChecked(this->_autoScroll);
        buffAction->setIconVisibleInMenu(true);
        connect(buffAction, SIGNAL(toggled(bool)), this, SLOT(autoScroll(bool)));

        menu->exec(this->mapToGlobal(pos));

        delete menu;
        menu = nullptr;
    }
}

void StatusBarInfoWidgetData::clickInCell(const QModelIndex &index)
{
    this->_currentIndex = index;
}

void StatusBarInfoWidgetData::doubleClickInCell(const QModelIndex &index)
{
    emit this->activateElement(index);
}

void StatusBarInfoWidgetData::deleteElement()
{
    emit this->removeElement(this->_currentIndex);
    this->_currentIndex = this->_model->index(this->_model->rowCount(), this->_model->columnCount());
}

void StatusBarInfoWidgetData::deleteAll()
{
    emit this->removeAllElements();
}

void StatusBarInfoWidgetData::autoScroll(bool value)
{
    this->_autoScroll = value;
}

void StatusBarInfoWidgetData::autoScroll()
{
    this->ui->tableView->resizeColumnToContents(1);
    this->ui->tableView->resizeColumnToContents(2);
    this->ui->tableView->resizeColumnToContents(3);

    if(this->_modelRows > this->_model->rowCount())
    {
        this->_modelRows = this->_model->rowCount();
        return;
    }

    if(this->_autoScroll == true)
        this->ui->tableView->scrollToBottom();
}
