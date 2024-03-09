#include "transhubwingow.h"

TransHubWindow::TransHubWindow(QWidget *parent) : QMainWindow(parent)
{
    elementsPrograms();
    setGeometry(0, 30, 1300, 800);

    setCentralWidget(left_screen);

    readSettings();
}

TransHubWindow::~TransHubWindow()
{
    writeSettins();
}

void TransHubWindow::elementsPrograms()
{
    left_screen = new LeftScreen(this);
    newFileAction = new QAction(tr("New file"), this);
    newFileAction->setShortcut(QKeySequence::New);
    newFileAction->setStatusTip(tr("Create a new file"));
    connect(newFileAction, &QAction::triggered, this, &TransHubWindow::newFile);

    openFileAction = new QAction(tr("Open file"), this);
    openFileAction->setShortcut(QKeySequence::Open);
    openFileAction->setStatusTip(tr("Open file"));
    connect(openFileAction, &QAction::triggered, this, &TransHubWindow::openFile);

    saveFileAction = new QAction(tr("Save file"), this);
    saveFileAction->setShortcut(QKeySequence::Save);
    saveFileAction->setStatusTip(tr("Save file"));
    connect(saveFileAction, &QAction::triggered, this, &TransHubWindow::saveFile);

    saveAsFileAction = new QAction(tr("Save As file"), this);
    saveAsFileAction->setShortcut(QKeySequence::SaveAs);
    saveAsFileAction->setStatusTip(tr("Save all"));
    connect(saveAsFileAction, &QAction::triggered, this, &TransHubWindow::saveAsFile);

    exitProgramAction = new QAction(tr("Exit program"), this);
    exitProgramAction->setShortcuts(QKeySequence::Close);
    exitProgramAction->setStatusTip(tr("exit program"));
    connect(exitProgramAction, &QAction::triggered, this, &TransHubWindow::exitProgram);

    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(saveAsFileAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitProgramAction);

    fileToolBar = new QToolBar(tr("&File"));
    addToolBar(Qt::LeftToolBarArea, fileToolBar);

    fileToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
    fileToolBar->addAction(newFileAction);
    fileToolBar->addAction(openFileAction);
    fileToolBar->addAction(saveFileAction);
    fileToolBar->addAction(saveAsFileAction);
    fileToolBar->addSeparator();
    fileToolBar->addAction(exitProgramAction);

    topToolBar = new QToolBar(tr("topBar"));
    addToolBar(Qt::TopToolBarArea, topToolBar);

    topToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

    comboBox_1 = new QComboBox();
    comboBox_1->addItem(tr("Combo_1"));
    comboBox_1->addItem(tr("Combo_2"));
    comboBox_1->addItem(tr("Combo_3"));
    comboBox_1->addItem(tr("Combo_4"));
    comboBox_1->addItem(tr("Combo_5"));

    comboBox_2 = new QComboBox();
    comboBox_2->addItem(tr("Combo_1"));
    comboBox_2->addItem(tr("Combo_2"));
    comboBox_2->addItem(tr("Combo_3"));
    comboBox_2->addItem(tr("Combo_4"));
    comboBox_2->addItem(tr("Combo_5"));

    checkBox_1 = new QCheckBox(tr("checkBox"));
    checkBox_1->setDisabled(false);

    pushButton = new QPushButton(tr("button"));

    topToolBar->addWidget(comboBox_1);
    topToolBar->addWidget(comboBox_2);
    topToolBar->addSeparator();
    topToolBar->addWidget(checkBox_1);
    topToolBar->addWidget(pushButton);

    tableWidget = new QTableWidget(100, 2);
    dockWidget = new QDockWidget(tr("QDockWidget"));
    dockWidget->setObjectName(tr("dockWidget"));

    dockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
    dockWidget->setWidget(tableWidget);


    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

void TransHubWindow::writeSettins()
{
    QSettings settings("Software Inc.", "texxtEditor");

    settings.beginGroup("mainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.endArray();
}

void TransHubWindow::readSettings()
{
    QSettings settings("Software Inc.", "texxtEditor");

    settings.beginGroup("mainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    settings.endArray();
}

void TransHubWindow::newFile()
{

}

void TransHubWindow::openFile()
{

}

void TransHubWindow::saveFile()
{

}

void TransHubWindow::saveAsFile()
{

}

void TransHubWindow::exitProgram()
{
    close();
}

