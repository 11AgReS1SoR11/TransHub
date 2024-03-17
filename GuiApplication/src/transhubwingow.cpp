//#include "transhubwingow.h"

//TransHubWindow::TransHubWindow(QWidget *parent) : QMainWindow(parent)
//{
//    elementsPrograms();
//    setGeometry(0, 30, 1300, 800);

//    setCentralWidget(map_screen);
//}

//TransHubWindow::~TransHubWindow()
//{
//}

//void TransHubWindow::elementsPrograms()
//{
//    map_screen = new MapScreen(this);
//    newFileAction = new QAction(tr("New file"), this);
//    newFileAction->setShortcut(QKeySequence::New);
//    newFileAction->setStatusTip(tr("Create a new file"));
//    connect(newFileAction, &QAction::triggered, this, &TransHubWindow::newFile);

//    openFileAction = new QAction(tr("Open file"), this);
//    openFileAction->setShortcut(QKeySequence::Open);
//    openFileAction->setStatusTip(tr("Open file"));
//    connect(openFileAction, &QAction::triggered, this, &TransHubWindow::openFile);

//    saveFileAction = new QAction(tr("Save file"), this);
//    saveFileAction->setShortcut(QKeySequence::Save);
//    saveFileAction->setStatusTip(tr("Save file"));
//    connect(saveFileAction, &QAction::triggered, this, &TransHubWindow::saveFile);

//    saveAsFileAction = new QAction(tr("Save As file"), this);
//    saveAsFileAction->setShortcut(QKeySequence::SaveAs);
//    saveAsFileAction->setStatusTip(tr("Save all"));
//    connect(saveAsFileAction, &QAction::triggered, this, &TransHubWindow::saveAsFile);

//    exitProgramAction = new QAction(tr("Exit program"), this);
//    exitProgramAction->setShortcuts(QKeySequence::Close);
//    exitProgramAction->setStatusTip(tr("exit program"));
//    connect(exitProgramAction, &QAction::triggered, this, &TransHubWindow::exitProgram);

//    fileMenu = menuBar()->addMenu(tr("File"));
//    fileMenu->addAction(newFileAction);
//    fileMenu->addAction(openFileAction);
//    fileMenu->addAction(saveFileAction);
//    fileMenu->addAction(saveAsFileAction);
//    fileMenu->addSeparator();
//    fileMenu->addAction(exitProgramAction);

//    fileToolBar = new RightToolBar(tr("&File"));
//    addToolBar(Qt::RightToolBarArea, fileToolBar);

//    topToolBar = new TopToolBar(tr("topBar"));
//    addToolBar(Qt::TopToolBarArea, topToolBar);

//    topToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);

//    tableWidget = new QTableWidget(100, 2);
//    dockWidget = new QDockWidget(tr("Сoordinates"));
//    dockWidget->setObjectName(tr("dockWidget"));

//    dockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
//    dockWidget->setWidget(tableWidget);


//    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
//    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
//}

//void TransHubWindow::newFile()
//{

//}

//void TransHubWindow::openFile()
//{

//}

//void TransHubWindow::saveFile()
//{

//}

//void TransHubWindow::saveAsFile()
//{

//}

//void TransHubWindow::exitProgram()
//{
//    writeSettings();
//    close();
//}

//void TransHubWindow::closeEvent(QCloseEvent *event) {
//        // Выполнение необходимых действий перед закрытием окна
//        writeSettings();

//        // Вызов базовой реализации метода closeEvent
//        QMainWindow::closeEvent(event);
//};

