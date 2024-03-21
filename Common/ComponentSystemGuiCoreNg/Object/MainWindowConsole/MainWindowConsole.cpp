#include "MainWindowConsole.h"
#include "ui_MainWindowConsole.h"
#include <QtWidgets/QApplication>
#include <QtCore/QDebug>

MainWindowConsole::MainWindowConsole(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindowConsole)
{
    ui->setupUi(this);

    _app = dynamic_cast<IApplication*> (qApp);
    if (_app) {
        connect (_app, SIGNAL(applicationLogData(QString,QString)),
                this, SLOT(applicationLogData(QString,QString)));
    } else {
        qCritical() << "[MainWindowConsole] Convert to IApplication failed! Log data will not be displayed in MainWindow!";
    }

    _model = new QStandardItemModel(this);

    _sortModel = new QSortFilterProxyModel(this);
    _sortModel->setSourceModel(_model);

    ui->tableView->setModel(_sortModel);
    ui->tableView->setItemDelegate(new TableColorDelegate(this));
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->toolButtonDebug->setIcon(QIcon(":/icons/icons/information-white.png"));
    ui->toolButtonDebug->setToolTip(tr("Debug"));

    ui->toolButtonWarning->setIcon(QIcon(":/icons/icons/exclamation.png"));
    ui->toolButtonWarning->setToolTip(tr("Warning"));

    ui->toolButtonCritical->setIcon(QIcon(":/icons/icons/exclamation-red.png"));
    ui->toolButtonCritical->setToolTip(tr("Critical"));

    ui->toolButtonAutoScroll->setIcon(QIcon(":/icons/icons/fill-270.png"));
    ui->toolButtonAutoScroll->setToolTip(tr("Проматывать в конец"));

    ui->toolButtonClear->setIcon(QIcon(":/icons/icons/broom.png"));
    ui->toolButtonClear->setToolTip(tr("Очистить список"));

    connect(ui->toolButtonDebug, SIGNAL(toggled(bool)), this, SLOT(clickDebug(bool)));
    connect(ui->toolButtonWarning, SIGNAL(toggled(bool)), this, SLOT(clickWarning(bool)));
    connect(ui->toolButtonCritical, SIGNAL(toggled(bool)), this, SLOT(clickCritical(bool)));
    connect(ui->toolButtonClear, SIGNAL(clicked()), this, SLOT(clickClear()));
    connect(_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateDataModel(QModelIndex,QModelIndex)));
    connect(ui->tableView, SIGNAL(pressed(QModelIndex)), this, SLOT(clickInCell(QModelIndex)));
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuView(QPoint)));

    this->loadWindowMode();

    this->initModel();
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->hideColumn(1);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setSectionResizeMode (0, QHeaderView::Stretch);

    if(ui->toolButtonAutoScroll->isChecked())
        ui->tableView->scrollToBottom();
}

MainWindowConsole::~MainWindowConsole()
{
    this->saveWindowModel();

    _app = nullptr;
    delete _sortModel;
    delete _model;
    delete ui;
}

void MainWindowConsole::loadWindowMode()
{
    QSettings settings (qApp->organizationName (), qApp->applicationName ());
    settings.beginGroup ("/MainWindowConsole");
    ui->toolButtonDebug->setChecked (settings.value ("showDebug", true).toBool());
    ui->toolButtonWarning->setChecked (settings.value ("showWarning", true).toBool());
    ui->toolButtonCritical->setChecked (settings.value ("showCritical", true).toBool());
    settings.endGroup();
}

void MainWindowConsole::saveWindowModel()
{
    QSettings settings (qApp->organizationName (), qApp->applicationName ());
    settings.beginGroup("/MainWindowConsole");
    settings.setValue("showDebug", ui->toolButtonDebug->isChecked());
    settings.setValue("showWarning", ui->toolButtonDebug->isChecked());
    settings.setValue("showCritical", ui->toolButtonDebug->isChecked());
    settings.endGroup();
}

void MainWindowConsole::initModel()
{
    QString regValue;
    if(ui->toolButtonDebug->isChecked())
        regValue = "Debug";

    if(ui->toolButtonWarning->isChecked()) {
        if(regValue.isEmpty())
            regValue = "Warning";
        else
            regValue = QString(regValue + "|Warning");
    }

    if(ui->toolButtonCritical->isChecked()) {
        if(regValue.isEmpty())
            regValue = "Critical";
        else
            regValue = QString(regValue + "|Critical");
    }

    if(regValue.isEmpty())
        regValue = QString("Nothing");

    regValue = QString("(" + regValue + ")");

    _sortModel->setFilterRegExp(QRegExp(regValue));
    _sortModel->setFilterKeyColumn(1);
}

void MainWindowConsole::applicationLogData (const QString &msgType, const QString &msg)
{
    if (!_model)
        return;

    QFont textFont(this->font().family(), this->font().pointSize());
    textFont.setBold(true);
    QBrush textBrush(Qt::red);

    QString msgData = QString (QDateTime::currentDateTime ().toString ("dd.MM.yyyy - hh:mm:ss") + "  " + msg);
    msgData = msgData.replace ("\n"," ");
    msgData = msgData.replace ("\t"," ");

    QStandardItem* buffItem = new QStandardItem();
    buffItem->setText(msgData);

    if(msgType == "Debug") {
        buffItem->setIcon(QIcon(":/icons/icons/information-white.png"));

    } else if(msgType == "Warning") {
        buffItem->setIcon(QIcon(":/icons/icons/exclamation.png"));
        buffItem->setFont(textFont);

    } else if(msgType == "Critical") {
        buffItem->setIcon(QIcon(":/icons/icons/exclamation-red.png"));
        buffItem->setFont(textFont);
        buffItem->setForeground(textBrush);
    }

    int row = _model->rowCount();
    _model->setItem(row, 0, buffItem);

    buffItem = new QStandardItem();
    buffItem->setText(msgType);
    _model->setItem(row, 1, buffItem);
}

void MainWindowConsole::clickDebug(bool value)
{
    Q_UNUSED(value)
    this->initModel();

    if(ui->toolButtonAutoScroll->isChecked())
        ui->tableView->scrollToBottom();
}

void MainWindowConsole::clickWarning(bool value)
{
    Q_UNUSED(value)
    this->initModel();

    if(ui->toolButtonAutoScroll->isChecked())
        ui->tableView->scrollToBottom();
}

void MainWindowConsole::clickCritical(bool value)
{
    Q_UNUSED(value)
    this->initModel();

    if(ui->toolButtonAutoScroll->isChecked())
        ui->tableView->scrollToBottom();
}

void MainWindowConsole::clickClear()
{
    if(_model) {
        _model->removeRows(0, 0, QModelIndex()); //чистим модель
        _model->setRowCount(0);
    }
}

void MainWindowConsole::clickCopy()
{
    QModelIndex buffIndex = _model->index(_currentIndex.row(), _currentIndex.column());
    if(buffIndex.isValid()) {
        QStandardItem* buffItem = _model->item(buffIndex.row(), buffIndex.column());
        if(buffItem)
            QApplication::clipboard()->setText(buffItem->text());
    }
}

void MainWindowConsole::clickInCell(const QModelIndex &index)
{
    _currentIndex = index;
}

void MainWindowConsole::contextMenuView(const QPoint &pos)
{
    QModelIndex buffIndex = _model->index(_currentIndex.row(), _currentIndex.column());
    if(buffIndex.isValid()) {
        QMenu* menu = new QMenu(this);
        QAction* buffAction = menu->addAction(QIcon(":/icons/icons/blue-document-copy.png"), tr("Копировать"));
        buffAction->setIconVisibleInMenu(true);
        connect(buffAction, SIGNAL(triggered()), this, SLOT(clickCopy()));

        menu->exec(this->mapToGlobal(pos));
        delete menu;
        menu = nullptr;
    }
}

void MainWindowConsole::updateDataModel(const QModelIndex &indexFrom, const QModelIndex &indexTo)
{
    Q_UNUSED(indexFrom)
    Q_UNUSED(indexTo)

    this->initModel();    
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->hideColumn(1);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setSectionResizeMode (0, QHeaderView::Stretch);

    if(ui->toolButtonAutoScroll->isChecked())
        ui->tableView->scrollToBottom();
}
