#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SystemGuiCoreNg.h"
#include "MainWindowDefines.h"

#include <QtCore/QUuid>
#include <QtCore/QSettings>
#include <QtCore/QtGlobal>
#include <QWidgetAction>

// делегаты
#include "Delegates/TableColorDelegate.h"
#include "Delegates/TableConvertDateTimeDelegate.h"
#include "Delegates/TableConvertDateDelegate.h"
#include "Delegates/TableBoolDelegate.h"

MainWindow::MainWindow (QWidget *parent)
    : QMainWindow (parent)
    , ui (new Ui::MainWindow)
{
    ui->setupUi (this);

    setContextMenuPolicy (Qt::NoContextMenu);
    setAttribute (Qt::WA_DeleteOnClose);

    _mainWindowConnector = new MainWindowConnector (this);
    connect (this, &MainWindow::subWindowClosed, _mainWindowConnector, &MainWindowConnector::subWindowClosed);
    connect (this, &MainWindow::subWindowOpened, _mainWindowConnector, &MainWindowConnector::subWindowOpened);

    _settingsPanelContainer = new SettingsPanelContainer (this);

    _userMode = ISystemGuiCore::USER_MODE;
    _useAppLoaderSettings = false;
    _isShowed = false;

    _minimizeToTray = false;
    _minimizeToTrayTemp = _minimizeToTray;

    //-- mdi area
    _mdiArea = new QMdiArea (this);

    //-- set filter
    _mdiAreaFilter = new MdiAreaFilter (_mdiArea);
    _mdiAreaFilter->setMdiArea (_mdiArea);
    connect (_mdiAreaFilter, &MdiAreaFilter::mousePos, this, &MainWindow::mousePos);
    _mdiArea->setMouseTracking (true);
    _mdiArea->installEventFilter (_mdiAreaFilter);
    // ---

    _mdiArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    _mdiArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    connect (_mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::updateMenus);
    setCentralWidget (_mdiArea);
    // end mdi area

    _windowMapper = new QSignalMapper (this);
    connect (_windowMapper, QOverload<QWidget*>::of (&QSignalMapper::mapped),
             this, &MainWindow::setActiveSubWindow);

    // status bar
    // --- set filter ---
    StatusBarFilter *statusBarFilter = new StatusBarFilter (ui->statusbar);
    connect (statusBarFilter, &StatusBarFilter::mouseEnter, this, &MainWindow::mouseStatusBarEnter);
    ui->statusbar->installEventFilter (statusBarFilter);
    // ---

    _statusBarInfoWidget = new StatusBarInfoWidget (this, this);
    _statusBarVolume = new StatusBarVolume (this, this);
    _statusBarMapWidget = new StatusBarMapWidget(this, this);
    _statusBarPlanningWidget = new StatusBarPlanningWidget(this, this);

    QAction* action = new QAction(this);

    _statusBarMapWidget->addAction(action);

    connect (_statusBarVolume, &StatusBarVolume::signalVolumeChanged, _statusBarInfoWidget, &StatusBarInfoWidget::slotVolumeChanged);

    connect (_statusBarVolume, &StatusBarVolume::signalHideToolTip, _statusBarInfoWidget, &StatusBarInfoWidget::slotHideToolTip);
    connect (_statusBarVolume, &StatusBarVolume::signalHideToolTip, _statusBarMapWidget, &StatusBarMapWidget::slotHideToolTip);
    connect (_statusBarVolume, &StatusBarVolume::signalHideToolTip, _statusBarPlanningWidget, &StatusBarPlanningWidget::slotHideToolTip);

    connect (_statusBarInfoWidget, &StatusBarInfoWidget::signalHideToolTip, _statusBarVolume, &StatusBarVolume::slotHideToolTip);
    connect (_statusBarInfoWidget, &StatusBarInfoWidget::signalHideToolTip, _statusBarMapWidget, &StatusBarMapWidget::slotHideToolTip);
    connect (_statusBarInfoWidget, &StatusBarInfoWidget::signalHideToolTip, _statusBarPlanningWidget, &StatusBarPlanningWidget::slotHideToolTip);

    connect (_statusBarMapWidget, &StatusBarMapWidget::signalHideToolTip, _statusBarInfoWidget, &StatusBarInfoWidget::slotHideToolTip);
    connect (_statusBarMapWidget, &StatusBarMapWidget::signalHideToolTip, _statusBarVolume, &StatusBarVolume::slotHideToolTip);
    connect (_statusBarMapWidget, &StatusBarMapWidget::signalHideToolTip, _statusBarPlanningWidget, &StatusBarPlanningWidget::slotHideToolTip);

    connect (_statusBarPlanningWidget, &StatusBarPlanningWidget::signalHideToolTip, _statusBarInfoWidget, &StatusBarInfoWidget::slotHideToolTip);
    connect (_statusBarPlanningWidget, &StatusBarPlanningWidget::signalHideToolTip, _statusBarVolume, &StatusBarVolume::slotHideToolTip);
    connect (_statusBarPlanningWidget, &StatusBarPlanningWidget::signalHideToolTip, _statusBarMapWidget, &StatusBarMapWidget::slotHideToolTip);


    ui->statusbar->addPermanentWidget(_statusBarMapWidget);
    ui->statusbar->addPermanentWidget(_statusBarPlanningWidget);
    ui->statusbar->addPermanentWidget(_statusBarVolume);
    ui->statusbar->addPermanentWidget(_statusBarInfoWidget);

    initDefaultMenuFile ();
}

MainWindow::~MainWindow ()
{
    {
        QMapIterator<QString, ExternalMainWindow *> iter (_externalMainWindows);
        while (iter.hasNext ()) {
            iter.next ();
            disconnect (iter.value (), &ExternalMainWindow::destroyed,
                        this, &MainWindow::slotExternalMainWindowDestroyed);
            iter.value ()->close ();
            delete iter.value ();
        }
        _externalMainWindows.clear ();
    }
    {
        QMapIterator<QString, ExternalDialog *> iter (_externalDialogs);
        while (iter.hasNext ()) {
            iter.next ();
            disconnect (iter.value (), &ExternalDialog::destroyed,
                        this, &MainWindow::slotExternalDialogDestroyed);
            iter.value ()->close ();
            delete iter.value ();
        }
        _externalDialogs.clear ();
    }

    delete ui;
}

bool MainWindow::initCentralWidget (QWidget *value)
{
    if (!value) return false;

    if (_mdiArea != nullptr) {
        if (_mdiAreaFilter != nullptr) {
            _mdiArea->removeEventFilter (_mdiAreaFilter);
            delete _mdiAreaFilter;
            _mdiAreaFilter = nullptr;
        }

        delete _mdiArea;
        _mdiArea = nullptr;
    }

    if (centralWidget ())
        return false;

    setCentralWidget (value);
    setContentsMargins(0, 0, 0, 0);
    return true;
}

bool MainWindow::isCentralWidget ()
{
    if (_mdiArea != nullptr)
        return false;
    return true;
}

void MainWindow::clearAllMainWindow ()
{
    //-- удаляем раздел в меню systemTrayIcon
    QAction* buffAction = getTrayIconAction();
    if (buffAction) {
        auto sti = _systemTrayIcon.lock ();
        if (sti && sti->contextMenu ())
            sti->contextMenu ()->removeAction (buffAction);
        buffAction = nullptr;
    }

    QMapIterator<ISystemGuiCoreBasePanel::BasePanelType, BasePanelWidget*>i(_basePanelsWidgets);
    while(i.hasNext()) {
        i.next();
        BasePanelWidget* buffW = i.value();
        delete buffW;
        buffW = nullptr;
    }
    _basePanelsWidgets.clear();

    QMapIterator<ISystemGuiCoreBasePanel::BasePanelType, BasePanelDockWidget*>j(_basePanels);
    while(j.hasNext()) {
        j.next();
        BasePanelDockWidget* buffW = j.value();
        delete buffW;
        buffW = nullptr;
    }
    _basePanels.clear();

    clearPopUps();

    delete _windowsCreator;
    _windowsCreator = nullptr;

    delete _statusBarInfoWidget;
    _statusBarInfoWidget = nullptr;

    delete _statusBarVolume;
    _statusBarVolume = nullptr;

    delete _mainWindowConnector;
    _mainWindowConnector = nullptr;

    delete _settingsPanelContainer;
    _settingsPanelContainer = nullptr;

    delete _windowMapper;
    _windowMapper = nullptr;

    if (_mdiArea) {
        delete _mdiArea;
        _mdiArea = nullptr;
    }

    _windowMenu = nullptr;
    _closeAct = nullptr;
    _closeAllAct = nullptr;
    _tileAct = nullptr;
    _cascadeAct = nullptr;
    _separatorAct = nullptr;

    _helpDesigner = nullptr;
    _systemGuiCore = nullptr;
    _iconManager = nullptr;
    _windowTitle.clear();
    _allWindowTypes.clear();
    _userLogin.clear();
}

void MainWindow::changeEvent (QEvent *event)
{
    if (event->type () != QEvent::ActivationChange) {
        QMainWindow::changeEvent (event);
        return;
    }

    QAction* buffAction = getTrayIconAction ();
    if (!buffAction) {
        QMainWindow::changeEvent (event);
        return;
    }

    if(isActiveWindow ())
        buffAction->setEnabled (false);
    else
        buffAction->setEnabled (true);

    QMainWindow::changeEvent (event);
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    auto act = getTrayIconAction ();
    if (!act) {
        saveWindowStyle ();
        emit mainWindowClosed ();
        event->accept ();
        return;
    }

    if (_minimizeToTrayTemp == true)
    {
        act->setEnabled (true);
        setVisible (false);

        auto sti = _systemTrayIcon.lock ();
        if (sti)
            sti->showMessage (_windowTitle,
                              QString (tr ("Окно \"") + _windowTitle +
                                       tr ("\" было свернуто в трей.\nДля открытия - выберите нужный пункт меню.")),
                              QSystemTrayIcon::Information,
                              5000);
        event->ignore ();
        return;

    }

    saveWindowStyle ();
    emit mainWindowClosed ();
    event->accept ();
}

void MainWindow::setSystemTrayIcon (const std::shared_ptr<QSystemTrayIcon> &systemTrayIcon) {
    _systemTrayIcon = systemTrayIcon;
}

void MainWindow::setIconManager (ISystemIconManager *iconManager) {
    _iconManager = iconManager;
}

void MainWindow::closeMainWindowTray () {
    _minimizeToTrayTemp = false;
}

void MainWindow::addWindowType (const QString &windowType) {
    _allWindowTypes.push_back (windowType);
}

void MainWindow::addWindowTypes (const QVector<QString> &windowTypes)
{
    for (int i = 0; i < windowTypes.size(); i++)
        _allWindowTypes.push_back(windowTypes[i]);
}

bool MainWindow::containsType (const QString &windowType) {
    return _allWindowTypes.contains(windowType);
}

QVector<QString> MainWindow::windowTypes () {
    return _allWindowTypes;
}

void MainWindow::setMainWindowTitle (const QString &windowTitle)
{
    _windowTitle = windowTitle;
    setWindowTitle (_windowTitle);
}

QString MainWindow::mainWindowTitle () {
    return _windowTitle;
}

void MainWindow::setSystemName (const QString &systemName) {
    _systemName = systemName;
}

void MainWindow::setUserMode (ISystemGuiCore::UserMode userMode) {
    _userMode = userMode;
}

void MainWindow::setUserLogin (const QString &userLogin) {
    _userLogin = userLogin;
}

void MainWindow::setSystemGuiCore (SystemGuiCoreNg *guiCore) {
    _systemGuiCore = guiCore;
}

void MainWindow::setUseAppLoaderSettings (bool isUse) {
    _useAppLoaderSettings = isUse;
}

void MainWindow::setHelpDesigner (HelpDesigner *helpDesigner) {
    _helpDesigner = helpDesigner;
}

void MainWindow::setMenuVisible (bool value)
{
    _menuVisible = value;
    if (ui->menubar)
        ui->menubar->setVisible (_menuVisible);
}

bool MainWindow::isMenuVisible () const {
    return _menuVisible;
}

void MainWindow::show ()
{
    init ();

    if (_mdiAreaFilter) {
        _mdiAreaFilter->setIconManager (_iconManager);
        _mdiAreaFilter->updateMdiBrush ();
    }

    QMainWindow::show ();
    QTimer::singleShot (1 * 1000, this, &MainWindow::showPopUps);
}

void MainWindow::hide () {
    QMainWindow::hide ();
}

bool MainWindow::isMdiMode () const {
    return _mdiArea == nullptr ? true : false;
}

void MainWindow::init ()
{
    _windowsCreator = new SystemWindowsCreator (_settingsPanelContainer, _iconManager, _windowTitle);

    initDefaultMenus ();
    initMenu ();
    initDefaultToolbars ();
    initToolBars ();
    initPanels ();
    initSettingsPanels ();
    loadWindowStyle ();

    if (_systemGuiCore && _systemGuiCore->settings ())
    {
        connect (_systemGuiCore->settings (),
                 QOverload<ISystemGuiCoreSettings::SettinsProperty, const QVariant &>::of (&ISystemGuiCoreSettings::settingsChanged),
                 this, [this](ISystemGuiCoreSettings::SettinsProperty property, QVariant value)
        {
            if (ISystemGuiCoreSettings::StatusBarVisible == property)
                ui->statusbar->setVisible (value.toBool ());
        });

        ui->statusbar->setVisible (_systemGuiCore->settings ()->statusBarVisible ());
    }
}

void MainWindow::initDefaultMenuFile ()
{
    auto fileMenu = getMenu (tr ("File"));
    if (fileMenu) {
        if (!addMenuInMenuBar (fileMenu))
            qWarning () << "[MainWindow][initDefaultMenuFile] Could not add 'File' menu";
    }
    else
        qWarning () << "[MainWindow][initDefaultMenuFile] Could not create 'File' menu";
}

void MainWindow::initDefaultMenus ()
{
    ISystemGuiCoreMenu* buffMenu = getMenu (tr ("File"));
    if (buffMenu) {
        buffMenu->addMenuSeparator ();
        QAction *buffAction = buffMenu->addMenuAction (tr ("Exit"),
                                                       QIcon (":/icons/icons/door-open-out.png"),
                                                       this,
                                                       ISystemGuiCoreMenu::NotUseConnect);
        connect (buffAction, &QAction::triggered, this, &MainWindow::clickAction);
    }

    SystemGuiCoreMenu *baseMenu = nullptr;

    {
        baseMenu = new SystemGuiCoreMenu (tr ("Tools"), ui->menubar);
        if (_mdiArea)
        {
            // добавляем меню Окна
            _windowMenu = new QMenu (tr ("Windows"), ui->menubar);
            baseMenu->addMenu(_windowMenu);

            _closeAct = new QAction (tr ("Close active"), this);
            _closeAct->setIcon (QIcon (""));
            _closeAct->setIconVisibleInMenu (true);
            connect (_closeAct, &QAction::triggered, _mdiArea, &QMdiArea::closeActiveSubWindow);

            _closeAllAct = new QAction (tr ("Close all"), this);
            _closeAllAct->setIcon (QIcon (""));
            _closeAllAct->setIconVisibleInMenu (true);
            connect (_closeAllAct, &QAction::triggered, _mdiArea, &QMdiArea::closeAllSubWindows);

            _tileAct = new QAction (tr ("Tile"), this);
            _tileAct->setIcon (QIcon (":/icons/icons/application-tile.png"));
            _tileAct->setIconVisibleInMenu (true);
            connect (_tileAct, &QAction::triggered, _mdiArea, &QMdiArea::tileSubWindows);

            _cascadeAct = new QAction (tr ("Cascade"), this);
            _cascadeAct->setIcon (QIcon (":/icons/icons/applications-stack.png"));
            _cascadeAct->setIconVisibleInMenu (true);
            connect (_cascadeAct, &QAction::triggered, _mdiArea, &QMdiArea::cascadeSubWindows);

            _separatorAct = new QAction (this);
            _separatorAct->setSeparator (true);

            connect (_windowMenu, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenu);
            updateWindowMenu ();
            updateMenus ();
        }

        //-- добавляем его несмотря на то, что он может быть пустым
        //-- потом меню Settings будет дополненно
        addMenuInMenuBar (baseMenu);
    }

    baseMenu = new SystemGuiCoreMenu (tr ("About ..."), ui->menubar);
    QAction* buffAction = baseMenu->addMenuAction (tr ("Components status"),
                                                   QIcon (":/icons/icons/componentsFolder16x16.png"),
                                                   _windowsCreator,
                                                   ISystemGuiCoreMenu::NotUseConnect);
    connect (buffAction, &QAction::triggered, this, &MainWindow::clickAction);

    buffAction = baseMenu->addMenuAction (tr ("About ..."),
                                          QIcon(":/icons/icons/information-white.png"),
                                          _windowsCreator,
                                          ISystemGuiCoreMenu::NotUseConnect);
    connect (buffAction, &QAction::triggered, this, &MainWindow::clickAction);

    if (_helpDesigner && !_helpDesigner->isEmpty ()) {
        buffAction = baseMenu->addMenuAction (tr ("Context help"),
                                              QIcon (":/icons/icons/question-white.png"),
                                              this,
                                              ISystemGuiCoreMenu::NotUseConnect);

        buffAction->setShortcut (QKeySequence (QKeySequence::HelpContents));
        connect (buffAction, &QAction::triggered, _helpDesigner, &HelpDesigner::showHelp);
    }

    buffAction = baseMenu->addMenuAction (tr ("About Qt"),
                                          QIcon (":/icons/icons/qt.png"),
                                          _windowsCreator,
                                          ISystemGuiCoreMenu::NotUseConnect);
    connect (buffAction, &QAction::triggered, this, &MainWindow::clickAboutQt);

    addMenuInMenuBar (baseMenu);

    if (ui->menubar)
        ui->menubar->setVisible (_menuVisible);
}

void MainWindow::initDefaultToolbars ()
{
}

void MainWindow::initToolBars ()
{
    //    {
    //        QList<SystemGuiCoreToolBar*> toolbars = findChildren<SystemGuiCoreToolBar*> ();
    //        qDebug () << "[MainWindow][initToolBars] gim mode" << toolbars.size ();
    //    }


    QList<QToolBar*> allToolbars = findChildren<QToolBar*> ();
    for (int i = 0; i < allToolbars.size(); i++)
    {
        SystemGuiCoreToolBar* buffToolBar = qobject_cast<SystemGuiCoreToolBar*> (allToolbars.operator [](i));
        if (buffToolBar)
        {
            QList <QAction*> buffActions = buffToolBar->actions ();
            for (int j = 0; j < buffActions.size(); j++)
            {
                if (buffActions[j]->property (ACTION_PROP_TYPE).toString () == ACTION_PROP_TYPE_USE_CONNECT)
                    connect(buffActions[j], &QAction::triggered, this, &MainWindow::clickAction);

                if (!buffActions[j]->property (ACTION_PROP_SIGNATURE).toString ().isEmpty ())
                    continue;

                buffActions[j]->setProperty (ACTION_PROP_SIGNATURE,
                                             QString ("TOOLBAR %1/%2").arg (buffToolBar->getTitle (), buffActions[j]->text ()));
            }
        }
    }
}

void MainWindow::initPanels ()
{
    if (_userMode == ISystemGuiCore::DEVELOPER_MODE) {
        ISystemGuiCoreBasePanel* buttonPanel = getPanel (ISystemGuiCoreBasePanel::BasePanelBottom);
        if(buttonPanel)
            buttonPanel->addTabWidget (tr ("Console"),
                                       QIcon (":/icons/icons/application-terminal.png"),
                                       new MainWindowConsole (this));
    }

    QMapIterator<ISystemGuiCoreBasePanel::BasePanelType, BasePanelWidget*>i(_basePanelsWidgets);
    while(i.hasNext()) {
        i.next();
        switch (i.key()) {
        case ISystemGuiCoreBasePanel::BasePanelLeft: {
            addPanel(i.key(), Qt::LeftDockWidgetArea, i.value());
            break;
        }
        case ISystemGuiCoreBasePanel::BasePanelRight: {
            addPanel(i.key(), Qt::RightDockWidgetArea, i.value());
            break;
        }
        case ISystemGuiCoreBasePanel::BasePanelBottom: {
            addPanel(i.key(), Qt::BottomDockWidgetArea, i.value());
            break;
        }
        default:
            break;
        }
    }
}

void MainWindow::addPanel (ISystemGuiCoreBasePanel::BasePanelType panelType,
                           Qt::DockWidgetArea type,
                           BasePanelWidget *widget)
{
    BasePanelDockWidget* dockW = new BasePanelDockWidget(this);
    //dockW->setFeatures(QDockWidget::DockWidgetMovable /*| QDockWidget::DockWidgetFloatable*/);
    dockW->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dockW->setWidget(widget);
    dockW->setDockWidgetArea(type);
    dockW->setMdiArea(_mdiArea);

    _basePanels.insert(panelType, dockW);
    connect (this, &MainWindow::mousePos, dockW, &BasePanelDockWidget::mousePos);
    connect (this, &MainWindow::mouseStatusBarEnter, dockW, &BasePanelDockWidget::showPanel);

    addDockWidget (type, dockW);
}

void MainWindow::initMenu ()
{
    SystemGuiCoreMenu* buffBaseMenu = qobject_cast<SystemGuiCoreMenu*> (findMenu (tr ("Tools")));
    if (buffBaseMenu) {
        QAction* buffActionParams = buffBaseMenu->addMenuAction (tr ("Options"),
                                                                 QIcon (":/icons/icons/"),
                                                                 _windowsCreator,
                                                                 ISystemGuiCoreMenu::NotUseConnect);

        connect (buffActionParams, &QAction::triggered, this, &MainWindow::clickAction);

        buffBaseMenu->addMenuSeparator ();
        QAction* buffAction = buffBaseMenu->addMenuAction (tr ("Minimize to tray"),
                                                           QIcon (""), //:/icons/icons/arrow-315.png
                                                           this,
                                                           ISystemGuiCoreMenu::NotUseConnect);
        buffAction->setCheckable (true);
        buffAction->setChecked (false);
        connect (buffAction, &QAction::toggled, this, &MainWindow::checkedAction);
    }

    sortMenuByNames ();
    initMenuSignature ();
    initMenuConnect ();
}

void MainWindow::sortMenuByNames ()
{
    QMap<QString, QMenu*> buffMap;
    QList<QAction*> buffList = ui->menubar->actions();
    for(int i = 0; i < buffList.size(); i++) {
        QMenu* buffMenu = buffList[i]->menu();
        if(buffMenu != nullptr)
            buffMap.insert(buffMenu->title(), buffMenu);
    }

    if (buffMap.contains (/*MENU_FILE*/tr ("File")))
        ui->menubar->addMenu (buffMap.value (/*MENU_FILE*/tr ("File")));

    QMapIterator<QString, QMenu*>j(buffMap);
    while(j.hasNext()) {
        j.next();
        if(j.value()->title() != /*MENU_FILE*/tr ("File") &&
                j.value()->title() != tr (/*MENU_TOOLS*/"Tools") &&
                j.value()->title() != tr (/*MENU_ABOUT_SYSTEM*/"About ..."))
            ui->menubar->addMenu(j.value());
    }

    if(buffMap.contains(tr (/*MENU_TOOLS*/"Tools")))
        ui->menubar->addMenu(buffMap.value(tr (/*MENU_TOOLS*/"Tools")));

    if(buffMap.contains(tr ("About ...")))
        ui->menubar->addMenu(buffMap.value(tr (/*MENU_ABOUT_SYSTEM*/"About ...")));
}

void MainWindow::initMenuConnect (QMenu *parentMenu)
{
    if (!parentMenu)
    {
        auto connectionslist = ui->menubar->actions ();
        for (QAction *act : qAsConst (connectionslist)) {
            if (act->menu () && !act->menu ()->actions ().isEmpty ())
                initMenuConnect (act->menu ());
        }
    }
    else
    {
        auto actionslist = parentMenu->actions ();
        for (QAction *act : qAsConst (actionslist))
        {
            if (act->menu () && !act->menu ()->actions ().isEmpty ())
                initMenuConnect (act->menu ());
            else {
                if (act->property (ACTION_PROP_TYPE).toString () == ACTION_PROP_TYPE_USE_CONNECT)
                    connect (act, &QAction::triggered, this, &MainWindow::clickAction);
            }
        }
    }
}

void MainWindow::initMenuSignature (QMenu *parentMenu, const QString &signaturePath)
{
    if (!parentMenu)
    {
        QList<QAction*> actions = ui->menubar->actions ();
        for (QAction *action : qAsConst (actions))
        {
            QMenu *menu = action->menu ();
            if (menu && menu->actions ().size () != 0)
                initMenuSignature (menu, menu->title ());
        }
    }
    else
    {
        QList<QAction*> actions = parentMenu->actions ();
        for (QAction *action: qAsConst (actions))
        {
            QMenu *menu = action->menu ();
            if (menu)
            {
                if (menu->actions ().size () != 0)
                    initMenuSignature (menu, QString ("%1/%2").arg (signaturePath).arg (menu->title ()));
            }
            else
            {
                if (action->isSeparator ())
                    continue;

                action->setProperty (ACTION_PROP_SIGNATURE, QString ("%1/%2").arg (signaturePath).arg (action->text ()));
            }
        }
    }
}

void MainWindow::initSettingsPanels ()
{
    //-- toolbars
    QList<SystemGuiCoreToolBar*> toolbarsList;
    QList<QToolBar*> allToolbars = findChildren <QToolBar*> ();
    for (int i = 0; i < allToolbars.size(); i++) {
        SystemGuiCoreToolBar* buffToolBar = qobject_cast<SystemGuiCoreToolBar*> (allToolbars[i]);
        if (buffToolBar)
            toolbarsList.push_back(buffToolBar);
    }

    if (toolbarsList.size () != 0) {
        SettingsToolBar* basePanel = new SettingsToolBar (toolbarsList);
        _settingsPanelContainer->addWidget (basePanel);
    }

    //-- base panels
    QMapIterator<ISystemGuiCoreBasePanel::BasePanelType, BasePanelWidget*> i (_basePanelsWidgets);
    while (i.hasNext ()) {
        i.next ();
        SettingsBasePanel* basePanel = new SettingsBasePanel (_basePanels.value (i.key()), i.value ());
        if (basePanel->initGui ())
            _settingsPanelContainer->addWidget (basePanel);
    }

    //-- настройки отображения SystemGuiCore
    _settingsPanelContainer->addWidget (new SettingsGuiCoreWidget (_systemGuiCore->settings (),
                                                                   _mdiArea, _tileAct, _cascadeAct));

    //-- gui loader
    if (_useAppLoaderSettings)
        _settingsPanelContainer->addWidget (new SettingsAppLoader());
}

void MainWindow::loadWindowStyle ()
{
    QSettings settings (qApp->organizationName (), qApp->applicationName ());
    settings.beginGroup (QString (QDir::separator () + _windowTitle));

    if (settings.value ("mainWindowMaximized", false).toBool () == false)  {
        resize (settings.value ("mainWindowSize", QSize (600,400)).toSize ());
        move (settings.value ("mainWindowPoz", QPoint (100,100)).toPoint ());
    } else {
        setWindowState (Qt::WindowMaximized);
        move (settings.value ("mainWindowPoz", QPoint (0,0)).toPoint ());
    }

    _statusBarVolume->setMute (settings.value ("systemSound", false).toBool ());
    _settingsPanelContainer->loadSettings (&settings);

    //-- меню "Инструменты"
    SystemGuiCoreMenu* buffMenu = qobject_cast<SystemGuiCoreMenu*>(findMenu (tr ("Tools")));
    if (buffMenu) {
        QAction *buffaction = buffMenu->getAction (tr ("Minimize to tray"));
        if (buffaction)
            buffaction->setChecked (settings.value ("minimizeToTray", _minimizeToTray).toBool ());
    }


    if (_systemGuiCore && _systemGuiCore->settings ())
        ui->statusbar->setVisible (_systemGuiCore->settings ()->statusBarVisible ());
}

void MainWindow::saveWindowStyle ()
{
    QSettings settings (qApp->organizationName (), qApp->applicationName ());
    settings.beginGroup (QDir::separator () + _windowTitle);

    //-- чистим все в данной ветке
    QStringList keys = settings.allKeys ();
    for (auto const &key : qAsConst (keys))
        settings.remove (key);

    settings.setValue ("mainWindowMaximized", isMaximized ());
    settings.setValue ("mainWindowSize", size ());
    settings.setValue ("mainWindowPoz", pos ());
    settings.setValue ("systemSound", _statusBarVolume->isMute ());

    _settingsPanelContainer->saveSettings (&settings);

    //-- сохраняем параметры меню "Инструменты"
    auto menuVid = qobject_cast<SystemGuiCoreMenu*> (findMenu (tr ("Tools")));
    if (menuVid) {
        auto act = menuVid->getAction (tr ("Minimize to tray"));
        if (act)
            settings.setValue ("minimizeToTray", act->isChecked ());
    }
}

void MainWindow::updateWindowMenu ()
{
    _windowMenu->clear();
    _windowMenu->addAction(_closeAct);
    _windowMenu->addAction(_closeAllAct);
    _windowMenu->addSeparator();
    _windowMenu->addAction(_tileAct);
    _windowMenu->addAction(_cascadeAct);
    _windowMenu->addAction(_separatorAct);
    //_windowMenu->setVisible(false);

    if (_mdiArea)
    {
        //_windowMenu->setVisible(true);

        QList<QMdiSubWindow *> windows = _mdiArea->subWindowList();
        _separatorAct->setVisible(!windows.isEmpty());

        for(int i = 0; i < windows.size(); ++i) {
            QMdiSubWindow *child = windows.at(i);

            QString text = windows.at(i)->windowTitle ();
            QIcon icon = windows.at(i)->windowIcon ();
            QAction *action  = _windowMenu->addAction (icon,text);
            action->setIconVisibleInMenu (true);
            action->setCheckable (true);
            action->setChecked (child == _mdiArea->activeSubWindow());
            connect (action, &QAction::triggered, _windowMapper, qOverload<> (&QSignalMapper::map));

            _windowMapper->setMapping (action, windows.at(i));
        }
    }
}

void MainWindow::updateMenus()
{
    bool hasMdiChild = false;
    if (_mdiArea && _mdiArea->activeSubWindow() != nullptr)
        hasMdiChild = true;

    _closeAct->setEnabled (hasMdiChild);
    _closeAllAct->setEnabled (hasMdiChild);

    if (_mdiArea && _mdiArea->viewMode() == QMdiArea::SubWindowView) {
        _tileAct->setEnabled (hasMdiChild);
        _cascadeAct->setEnabled (hasMdiChild);
    } else {
        _tileAct->setEnabled (false);
        _cascadeAct->setEnabled (false);
    }
    _separatorAct->setEnabled (hasMdiChild);
}

void MainWindow::setActiveSubWindow (QWidget *window)
{
    if (!window)
        return;
    _mdiArea->setActiveSubWindow (qobject_cast<QMdiSubWindow *> (window));
}

void MainWindow::slotCloseSubWindow ()
{
    QString actionSignature = sender ()->property (ACTION_PROP_SIGNATURE).toString ();
    emit subWindowClosed (actionSignature);
}

void MainWindow::slotExternalDialogDestroyed (QObject *object)
{
    QMapIterator<QString,ExternalDialog*> iter (_externalDialogs);
    while (iter.hasNext ()) {
        iter.next ();
        if (iter.value () == object) {
            _externalDialogs.remove (iter.key ());
            return;
        }
    }
}

void MainWindow::slotExternalMainWindowDestroyed (QObject *object)
{
    QMapIterator<QString,ExternalMainWindow*> iter (_externalMainWindows);
    while (iter.hasNext ()) {
        iter.next ();
        if (iter.value () == object) {
            _externalMainWindows.remove (iter.key ());
            return;
        }
    }
}

void MainWindow::clickAboutQt () {
    QMessageBox::aboutQt (this, tr ("About Qt"));
}

void MainWindow::clickAction ()
{
    setCursor (Qt::WaitCursor);

    //-- запрашиваем отправителя сигнала
    auto act = qobject_cast<QAction*> (sender ());
    if (!act) {
        unsetCursor ();
        qCritical () << "[MainWindow][clickAction] Cast to QAction failed!";
        return;
    }

    QString actionSignature = act->property (ACTION_PROP_SIGNATURE).toString ();
    if (actionSignature == QString ("%1/%2").arg (tr ("File"), tr ("Exit"))) {
        unsetCursor ();
        emit exitFromSystem ();
        return;
    }

    //-- ищем окно во внутренних окнах
    auto buffSubwindow = findMdiSubWindow (actionSignature);
    if (buffSubwindow != nullptr) {
        unsetCursor ();
        buffSubwindow->setFocus ();
        return;
    }

    //-- ищем окно во внешних окнах
    auto dlg = _externalMainWindows.value (actionSignature, nullptr);
    if (dlg != nullptr) {
        unsetCursor ();
        dlg->setFocus ();
        return;
    }

    //-- ищем окно во внешних виджетах
    auto dlgw = _externalDialogs.value (actionSignature, nullptr);
    if (dlgw != nullptr) {
        unsetCursor ();
        dlgw->setFocus ();
        return;
    }

    //-- нигде не нашли, открываем
    if (auto buffParent = dynamic_cast<ISystemGuiCoreParentWidget*> (act->parent ()))
    {
        ISystemGuiCoreParentWidget::WidgetType type = ISystemGuiCoreParentWidget::MdiType;

        ISystemGuiCoreParentWidget::WidgetShowType showtype;
        if(act->text() == "Map")
        {
            showtype = ISystemGuiCoreParentWidget::ShowMaximized;
        }
        else showtype = ISystemGuiCoreParentWidget::ShowNormal;

        QWidget* buffWidget = buffParent->getWidget (act->text (), actionSignature, type, showtype);

        if (!buffWidget) {
            //qWarning () << "[MainWindow][clickAction] The returned pointer to the widget is empty for action signature:"
            //            << actionSignature;
            unsetCursor ();
            return;
        }

        if (type == ISystemGuiCoreParentWidget::ExternalMainWindowType)
        {
            ExternalMainWindow *dlg = new ExternalMainWindow (buffWidget, nullptr);
            dlg->setWindowFlag (Qt::WindowMaximizeButtonHint, true);
            dlg->setWindowFlag (Qt::WindowMinimizeButtonHint, true);
            dlg->setWindowFlag (Qt::WindowSystemMenuHint, true);
            if (dlg->windowIcon ().isNull ())
                dlg->setWindowIcon (windowIcon ());
            dlg->setObjectName (actionSignature);
            dlg->setAttribute (Qt::WA_DeleteOnClose);

            connect (dlg, &ExternalMainWindow::destroyed,
                     this, &MainWindow::slotExternalMainWindowDestroyed);
            _externalMainWindows.insert (actionSignature, dlg);

            if (showtype == ISystemGuiCoreParentWidget::ShowNormal)
                dlg->show ();
            else if (showtype == ISystemGuiCoreParentWidget::ShowMaximized)
                dlg->showMaximized ();
            else if (showtype == ISystemGuiCoreParentWidget::ShowMinimized)
                dlg->showMinimized ();
            else if (showtype == ISystemGuiCoreParentWidget::ShowFullScreen)
                dlg->showFullScreen ();
        }
        else if (type == ISystemGuiCoreParentWidget::ExternalWidgetType)
        {
            ExternalDialog *dlg = new ExternalDialog (buffWidget, this);
            dlg->setWindowFlag (Qt::WindowMaximizeButtonHint, true);
            dlg->setWindowFlag (Qt::WindowMinimizeButtonHint, true);
            dlg->setWindowFlag (Qt::WindowSystemMenuHint, true);
            if (dlg->windowIcon ().isNull ())
                dlg->setWindowIcon (windowIcon ());
            dlg->setObjectName (actionSignature);
            dlg->setAttribute (Qt::WA_DeleteOnClose);

            connect (dlg, &ExternalDialog::destroyed,
                     this, &MainWindow::slotExternalDialogDestroyed);
            _externalDialogs.insert (actionSignature, dlg);

            if (showtype == ISystemGuiCoreParentWidget::ShowNormal)
                dlg->show ();
            else if (showtype == ISystemGuiCoreParentWidget::ShowMaximized)
                dlg->showMaximized ();
            else if (showtype == ISystemGuiCoreParentWidget::ShowMinimized)
                dlg->showMinimized ();
            else if (showtype == ISystemGuiCoreParentWidget::ShowFullScreen)
                dlg->showFullScreen ();
        }
        else if (type == ISystemGuiCoreParentWidget::MdiType)
        {
            if (_mdiArea == nullptr)
            {
                ExternalDialog *dlg = new ExternalDialog (buffWidget, this);
                dlg->setWindowFlag (Qt::WindowMaximizeButtonHint, true);
                dlg->setWindowFlag (Qt::WindowMinimizeButtonHint, true);
                dlg->setWindowFlag (Qt::WindowSystemMenuHint, true);
                dlg->setObjectName (actionSignature);
                dlg->setAttribute (Qt::WA_DeleteOnClose);

                connect (dlg, &ExternalDialog::destroyed,
                         this, &MainWindow::slotExternalDialogDestroyed);
                _externalDialogs.insert (actionSignature, dlg);

                if (showtype == ISystemGuiCoreParentWidget::ShowNormal)
                    dlg->show ();
                else if (showtype == ISystemGuiCoreParentWidget::ShowMaximized)
                    dlg->showMaximized ();
                else if (showtype == ISystemGuiCoreParentWidget::ShowMinimized)
                    dlg->showMinimized ();
                else if (showtype == ISystemGuiCoreParentWidget::ShowFullScreen)
                    dlg->showFullScreen ();
            }
            else
            {
                buffWidget->setAccessibleName (actionSignature);
                buffWidget->setAccessibleDescription (SUBWINDOW_TYPE_ACTION);


                ((QMdiArea*)centralWidget())->cascadeSubWindows();
                QMdiSubWindow* subWindow = ((QMdiArea*)centralWidget ())->addSubWindow (buffWidget);
                subWindow->setAccessibleName(actionSignature);
                subWindow->setAccessibleDescription(SUBWINDOW_TYPE_ACTION);
                subWindow->setWindowIcon(act->icon());
                subWindow->resize(buffWidget->size());
                buffWidget->setParent(subWindow);

                connect (buffWidget, &QWidget::destroyed, subWindow, &QMdiSubWindow::close);
                connect (subWindow, &QMdiSubWindow::destroyed, this, &MainWindow::slotCloseSubWindow);

                // --- set filter ---
                MdiSubWindowFilter *buffFilter = new MdiSubWindowFilter (subWindow, buffWidget);
                connect (buffFilter, &MdiSubWindowFilter::mousePos, this, &MainWindow::mousePos);
                buffWidget->setMouseTracking (true);
                buffWidget->installEventFilter (buffFilter);
                // ---

                if (actionSignature == QString ("%1/%2").arg (tr ("About ..."), tr ("About ..."))) {
                    subWindow->setFixedSize (buffWidget->size().width()+10, buffWidget->size().height()+10);
                    subWindow->setWindowFlags (Qt::WindowCloseButtonHint);
                }

                if (showtype == ISystemGuiCoreParentWidget::ShowNormal)
                    buffWidget->show ();
                else if (showtype == ISystemGuiCoreParentWidget::ShowMaximized)
                {
                    buffWidget->setMaximumSize(((QMdiArea*)centralWidget())->size());
                    buffWidget->showMaximized ();
                }
                else if (showtype == ISystemGuiCoreParentWidget::ShowMinimized)
                    buffWidget->showMinimized ();
                else if (showtype == ISystemGuiCoreParentWidget::ShowFullScreen)
                    buffWidget->showFullScreen ();

                emit subWindowOpened (subWindow->accessibleName());
            }
        }
    }
    else
    {
        bool isDisconnect = disconnect (act, &QAction::triggered, this, &MainWindow::clickAction);
        if (!isDisconnect)
            qWarning () << "[MainWindow][clickAction] Error disconnect"
                        << act->text ();
    }

    unsetCursor ();
}

void MainWindow::checkedAction (bool value)
{
    setCursor (Qt::WaitCursor);
    QAction* buffAction = qobject_cast<QAction*> (sender ());
    if (!buffAction) {
        //bool isDisconnect = disconnect (sender (), &QAction::triggered, this, &MainWindow::clickAction);
        //qCritical() << "[MainWindow][checkedAction] Convert to QAction failed! QObject::disconnect:"
        //            << isDisconnect;
        return;
    }

    QString actionSignature = buffAction->property (ACTION_PROP_SIGNATURE).toString ();
    if (actionSignature == tr ("%1/%2").arg (tr ("Tools"), tr ("Minimize to tray"))) {
        _minimizeToTray = value;
        _minimizeToTrayTemp = value;
        setCursor(Qt::ArrowCursor);
        return;
    }
    setCursor( Qt::ArrowCursor);
}

void MainWindow::loadAddedMdiAreaWindow (const QString &windowName)
{
    if (_basePanels.contains (ISystemGuiCoreBasePanel::BasePanelLeft)) {
        if (loadAddedWindow (windowName, ISystemGuiCoreBasePanel::BasePanelLeft))
            return;
    }
    if (_basePanels.contains (ISystemGuiCoreBasePanel::BasePanelRight)) {
        if (loadAddedWindow (windowName, ISystemGuiCoreBasePanel::BasePanelRight))
            return;
    }
    if (_basePanels.contains (ISystemGuiCoreBasePanel::BasePanelBottom)) {
        if(loadAddedWindow (windowName, ISystemGuiCoreBasePanel::BasePanelBottom))
            return;
    }
}

bool MainWindow::loadAddedWindow (const QString &windowName, ISystemGuiCoreBasePanel::BasePanelType panel)
{
    QTabWidget* tabWidget = _basePanelsWidgets.value (panel)->getTabWidget ();
    for (int i = 0; i < tabWidget->count(); i++)
    {
        ISystemGuiCoreParentWidget* parentWidget = dynamic_cast<ISystemGuiCoreParentWidget*>(tabWidget->widget(i));
        if (parentWidget) {
            ISystemGuiCoreParentWidget::WidgetType type = ISystemGuiCoreParentWidget::MdiType;
            ISystemGuiCoreParentWidget::WidgetShowType showtype =
                    ISystemGuiCoreParentWidget::ShowNormal;
            QWidget* buffWidget = parentWidget->getWidget (windowName, "", type, showtype);
            if (buffWidget) {
                buffWidget->setParent(this);
                buffWidget->setAccessibleName(windowName);
                buffWidget->setAccessibleDescription(SUBWINDOW_TYPE_ADD_IN_MDI);

                QMdiSubWindow* buffWIndow = ((QMdiArea*)centralWidget ())->addSubWindow (buffWidget);
                buffWIndow->setProperty(ACTION_PROP_SIGNATURE, windowName); // <--
                buffWIndow->setAccessibleName(windowName);
                buffWIndow->setAccessibleDescription(SUBWINDOW_TYPE_ADD_IN_MDI);
                buffWIndow->resize(buffWidget->size());
                buffWIndow->setWindowIcon(buffWidget->windowIcon());

                connect (buffWidget, &QWidget::destroyed, buffWIndow, &QMdiSubWindow::close);
                connect (buffWIndow, &QWidget::destroyed, this, &MainWindow::slotCloseSubWindow);

                // --- set filter ---
                MdiSubWindowFilter *buffFilter = new MdiSubWindowFilter(buffWIndow, buffWidget);
                connect (buffFilter, &MdiSubWindowFilter::mousePos, this, &MainWindow::mousePos);
                buffWidget->setMouseTracking(true);
                buffWidget->installEventFilter(buffFilter);
                // ---

                if (showtype == ISystemGuiCoreParentWidget::ShowNormal)
                    buffWidget->show ();
                else if (showtype == ISystemGuiCoreParentWidget::ShowMaximized)
                    buffWidget->showMaximized ();
                else if (showtype == ISystemGuiCoreParentWidget::ShowMinimized)
                    buffWidget->showMinimized ();
                else if (showtype == ISystemGuiCoreParentWidget::ShowFullScreen)
                    buffWidget->showFullScreen ();

                emit subWindowOpened(buffWIndow->accessibleName());
                return true;
            }
        }
    }
    return false;
}

QMenu* MainWindow::findMenu (const QString &menuName, QMenu *parentMenu)
{
    if(!parentMenu) {
        QList<QAction*> buffList = ui->menubar->actions();
        for(int i = 0; i < buffList.size(); i++) {
            QMenu* buffMenu = buffList[i]->menu();
            if(buffMenu) {
                if(buffMenu->title() == menuName)
                    return buffMenu;

                if(buffMenu->actions().size() != 0) {
                    QMenu* retMenu = findMenu(menuName, buffMenu);
                    if(retMenu)
                        return retMenu;
                }
            }
        }
    } else {
        QList<QAction*> buffList = parentMenu->actions();
        for(int i = 0; i < buffList.size(); i++) {
            QMenu* buffMenu = buffList[i]->menu();
            if(buffMenu) {
                if(buffMenu->title() == menuName)
                    return buffMenu;

                if(buffMenu->actions().size() != 0) {
                    QMenu* retMenu = findMenu(menuName, buffMenu);
                    if(retMenu)
                        return retMenu;
                }
            }
        }
    }
    return nullptr;
}

QToolBar *MainWindow::findToolbar (const QString &toolbarName)
{
    QList<QToolBar*> allToolbars = findChildren<QToolBar*>();
    for(int i = 0; i < allToolbars.size(); i++) {
        SystemGuiCoreToolBar* buffToolBar = qobject_cast<SystemGuiCoreToolBar*>(allToolbars[i]);
        if(buffToolBar) {
            if(buffToolBar->getTitle() == toolbarName)
                return allToolbars[i];
        }
    }
    return nullptr;
}

QAction *MainWindow::findMenuAction (const QString &actionName, QMenu *parentMenu)
{
    if(!parentMenu) {
        QList<QAction*> buffList = ui->menubar->actions();
        for(int i = 0; i < buffList.size(); i++) {
            QMenu* buffMenu = buffList[i]->menu();
            if(buffMenu) {
                if(buffMenu->actions().size() != 0) {
                    QAction* retAction = findMenuAction(actionName, buffMenu);
                    if(retAction)
                        return retAction;
                }
            } else {
                if(buffList[i]->text() == actionName)
                    return buffList[i];
            }
        }
    } else {
        QList<QAction*> buffList = parentMenu->actions();
        for(int i = 0; i < buffList.size(); i++) {
            QMenu* buffMenu = buffList[i]->menu();
            if(buffMenu) {
                if(buffMenu->actions().size() != 0) {
                    QAction* retAction = findMenuAction(actionName, buffMenu);
                    if(retAction)
                        return retAction;
                }
            } else {
                if(buffList[i]->text() == actionName)
                    return buffList[i];
            }
        }
    }
    return nullptr;
}

QAction *MainWindow::findMenuActionSignature (const QString &signaturePath, QMenu *parentMenu)
{
    QStringList sPathList = signaturePath.split ("/");
    if (sPathList.size() == 0)
        return nullptr;

    QString signaturePathNext;
    for(int i = 1; i < sPathList.size(); i++) {
        if(signaturePathNext.isEmpty())
            signaturePathNext = sPathList[i];
        else
            signaturePathNext = QString("%1/%2").arg(signaturePathNext).arg(sPathList[i]);
    }

    if(!parentMenu) {
        // ищем меню
        QList<QAction*> buffList = ui->menubar->actions();
        for(int i = 0; i < buffList.size(); i++) {
            QMenu* buffMenu = buffList[i]->menu();
            if(buffMenu) {
                if(buffMenu->title() != sPathList[0])
                    continue;

                if(buffMenu->actions().size() != 0) {
                    QAction* retAction = findMenuActionSignature(signaturePathNext, buffMenu);
                    if(retAction)
                        return retAction;
                }
            }
        }
    } else {
        // ищем подменю или QAction
        QList<QAction*> buffList = parentMenu->actions();
        for(int i = 0; i < buffList.size(); i++) {
            QMenu* buffMenu = buffList[i]->menu();
            if(buffMenu) {
                if(buffMenu->title() != sPathList[0])
                    continue;

                if(buffMenu->actions().size() != 0) {
                    QAction* retAction = findMenuActionSignature(signaturePathNext, buffMenu);
                    if(retAction)
                        return retAction;
                }
            } else {
                if(buffList[i]->text() == sPathList[0]) {
                    //                    qDebug() << "---> found: " << buffList[i]->text() << " Test: " << sPathList[0];
                    return buffList[i];
                }
            }
        }
    }
    return nullptr;
}

QAction *MainWindow::findToolbarsAction (const QString &actionName)
{
    QList<QToolBar*> allToolbars = findChildren<QToolBar*>();
    for(int i = 0; i < allToolbars.size(); i++) {
        QToolBar* buffToolBar = allToolbars[i];
        QList<QAction*> buffActionList = buffToolBar->actions();
        for(int j = 0; j < buffActionList.size(); j++) {
            if(buffActionList[j]->text() == actionName)
                return buffActionList[j];
        }
    }
    return nullptr;
}

QAction *MainWindow::findToolbarsActionSignature (const QString &signaturePath)
{
    QString buffSignaturePath = signaturePath;
    buffSignaturePath = buffSignaturePath.replace("TOOLBAR ","");
    QStringList sPathList = buffSignaturePath.split("/");
    if(sPathList.size() != 2)
        return nullptr;

    QList<QToolBar*> allToolbars = findChildren<QToolBar*>();
    for(int i = 0; i < allToolbars.size(); i++) {
        SystemGuiCoreToolBar* buffToolBar = qobject_cast<SystemGuiCoreToolBar*>(allToolbars[i]);
        if(!buffToolBar)
            continue;

        if(buffToolBar->getTitle() != sPathList[0])
            continue;

        QList<QAction*> buffActionList = buffToolBar->actions();
        for(int j = 0; j < buffActionList.size(); j++) {
            if(buffActionList[j]->text() == sPathList[1])
                return buffActionList[j];
        }
    }
    return nullptr;
}

QMdiSubWindow *MainWindow::findMdiSubWindow (const QString &windowName)
{
    if (!_mdiArea)
        return nullptr;

    auto list = _mdiArea->subWindowList ();
    for (QMdiSubWindow *subwindow : qAsConst (list)) {
        if (subwindow->accessibleName () == windowName)
            return subwindow;
    }

    return nullptr;
}

QAction *MainWindow::getTrayIconAction ()
{
    auto sti = _systemTrayIcon.lock ();
    if (!sti)
        return nullptr;

    QMenu *contextMenu = sti->contextMenu ();
    if (!contextMenu)
        return nullptr;

    auto actions = contextMenu->actions ();
    for (auto const &act : qAsConst (actions)) {
        if (act->text () == _windowTitle)
            return act;
    }
    return nullptr;
}

int MainWindow::getNumActionWindows()
{
    int num = 0;
    QList<QMdiSubWindow*> allMdi = _mdiArea->subWindowList();
    for(int i = 0; i < allMdi.size(); i++) {
        if(allMdi[i]->accessibleDescription() == QString(SUBWINDOW_TYPE_ACTION))
            num++;
    }
    return num;
}

int MainWindow::getNumAddInMdiWindows()
{
    int num = 0;
    QList<QMdiSubWindow*> allMdi = _mdiArea->subWindowList();
    for(int i = 0; i < allMdi.size(); i++) {
        if(allMdi[i]->accessibleDescription() == QString(SUBWINDOW_TYPE_ADD_IN_MDI))
            num++;
    }
    return num;
}

void MainWindow::showPopUps ()
{
    _isShowed = true;
    for (int i = 0; i < _popUps.size(); i++) {
        if (_statusBarInfoWidget) {
            _statusBarInfoWidget->addStatusElement(_popUps[i]->_popUp, _popUps[i]->_autoClose);
        } else {
            delete _popUps[i]->_popUp;
            _popUps[i]->_popUp = nullptr;
        }
    }
    clearPopUps();
}

void MainWindow::clearPopUps ()
{
    for (int i = 0; i < _popUps.size (); i++) {
        delete _popUps[i];
        _popUps[i] = nullptr;
    }
    _popUps.clear ();
}

// -- ISystemGuiCoreMainWindow ---------------------------------------------------------------------------
ISystemGuiCoreMenu* MainWindow::getMenu (const QString &menuName)
{
    QMutexLocker lock (&_mutex);

    if (menuName.isEmpty ()) {
        qCritical () << "[MainWindow][getMenu] Menu name is Empty! Get menu failed!";
        return nullptr;
    }

    if (menuName == tr ("Left panel") ||
            menuName == tr ("Right panel") ||
            menuName == tr ("Workspace") ||
            menuName == tr ("Bottom panel") ||
            menuName == tr ("Windows") ||
            menuName == tr ("Tools") ||
            menuName == tr ("About ...") ||
            menuName == tr ("Toolbars")) {
        qCritical () << "[MainWindow][getMenu] This menu name is reserved! Get menu failed!"
                     << menuName;
        return nullptr;
    }

    QMenu* buffMenu = findMenu (menuName);
    if (buffMenu) {
        SystemGuiCoreMenu* retMenu = qobject_cast<SystemGuiCoreMenu*> (buffMenu);
        return retMenu;
    }

    SystemGuiCoreMenu* retMenu = new SystemGuiCoreMenu (menuName, this);
    return retMenu;
}

bool MainWindow::addMenuInMenuBar (ISystemGuiCoreMenu *menu)
{
    QMutexLocker locker (&_mutex);

    if (!menu) {
        qCritical () << "[MainWindow][addMenuInMenuBar] Menu is NULL! Add failed!";
        return false;
    }

    SystemGuiCoreMenu *buffMenu = dynamic_cast<SystemGuiCoreMenu*> (menu);
    if (!buffMenu) {
        qCritical () << "[MainWindow][addMenuInMenuBar] Convert to SystemGuiCoreMenu failed! Add failed!";
        return false;
    }

    if (!buffMenu->isAdded () && !buffMenu->isSubMenu ()) {
        ui->menubar->addMenu (buffMenu);
        buffMenu->addedInMenuBar (true);
        return true;
    } else
        return false;
}

ISystemGuiCoreToolBar* MainWindow::getToolBar (const QString &toolBarName)
{
    QMutexLocker locker (&_mutex);

    if (toolBarName.isEmpty ()) {
        qCritical () << "[MainWindow][getToolBar] ToolBar name is Empty! Get toolbar faied!";
        return nullptr;
    }

    auto buffToolBar = findToolbar (toolBarName);
    if (buffToolBar) {
        SystemGuiCoreToolBar *retToolBar = qobject_cast<SystemGuiCoreToolBar*> (buffToolBar);
        return retToolBar;
    }

    auto retToolBar = new SystemGuiCoreToolBar (toolBarName, this);
    addToolBar (retToolBar);
    return retToolBar;
}

ISystemGuiCoreBasePanel *MainWindow::getPanel (ISystemGuiCoreBasePanel::BasePanelType panelType)
{
    QMutexLocker a (&_mutex);

    if(panelType != ISystemGuiCoreBasePanel::BasePanelLeft
            && panelType != ISystemGuiCoreBasePanel::BasePanelRight
            && panelType != ISystemGuiCoreBasePanel::BasePanelBottom) {
        qCritical() << "[MainWindow][getPanel] Undefined panel type! Get panel failed!";
        return nullptr;
    }

    if(_basePanelsWidgets.contains(panelType))
        return _basePanelsWidgets.value(panelType);

    BasePanelWidget* basePanelW = new BasePanelWidget(panelType, this);
    _basePanelsWidgets.insert(panelType, basePanelW);
    return basePanelW;
}

QStatusBar* MainWindow::getStatusBar()
{
    //QMutexLocker a(&_mutex);
    //Q_UNUSED(a)
    return ui->statusbar;
}

bool MainWindow::addSettingsWidget (ISystemGuiCoreSettingsPanelWidget *widget)
{
    //    QMutexLocker a (&_mutex);
    //    Q_UNUSED(a)

    if (!_settingsPanelContainer) {
        qCritical () << "[MainWindow][addSettingsWidget] Panel container is NULL! Add settings widget failed!";
        return false;
    }

    if (!widget) {
        qCritical() << "[MainWindow][addSettingsWidget] Widget is NULL! Add settings widget failed!";
        return false;
    }

    return _settingsPanelContainer->addWidget (widget);
}

bool MainWindow::removeSettingsWidget (ISystemGuiCoreSettingsPanelWidget *widget)
{
    //    QMutexLocker a (&_mutex);
    //    Q_UNUSED(a)

    if (!_settingsPanelContainer) {
        qCritical() << "[MainWindow][removeSettingsWidget] Panel container is NULL! Remove settings widget failed!";
        return false;
    }

    if (!widget) {
        qCritical() << "[MainWindow][removeSettingsWidget] Widget is NULL! Remove settings widget failed!";
        return false;
    }

    return _settingsPanelContainer->removeWidget (widget);
}

bool MainWindow::mdiSubWindowSetFocus (const QString &windowName)
{
    //    QMutexLocker a(&_mutex);
    //    Q_UNUSED(a)

    QMdiSubWindow* subWindow = findMdiSubWindow (windowName);
    if (!subWindow)
        return false;

    _mdiArea->setActiveSubWindow (subWindow);
    return true;
}

bool MainWindow::mdiSubWindowClose (QWidget *window)
{
    QMutexLocker a(&_mutex);
    Q_UNUSED(a)

    if (!window) {
        qCritical() << "[MainWindow][mdiSubWindowClose] Window is NULL! Close subwindow failed!";
        return false;
    }

    QMdiSubWindow* subWindow = findMdiSubWindow(window->accessibleName());
    if(!subWindow)
        return false;

    _mdiArea->removeSubWindow(subWindow->widget());
    subWindow->deleteLater();
    return true;
}

int MainWindow::userMode () {
    return _userMode;
}

QString MainWindow::getUserLogin () const {
    return _userLogin;
}

void MainWindow::addWindowInMdiArea (const QString &actionName, QWidget *window)
{
    //-- ищем окно во внутренних окнах
    auto buffSubwindow = findMdiSubWindow (actionName);
    if (buffSubwindow != nullptr) {
        unsetCursor ();
        buffSubwindow->setFocus ();
        return;
    }

    //-- ищем окно во внешних окнах
    auto dlg = _externalMainWindows.value (actionName, nullptr);
    if (dlg != nullptr) {
        unsetCursor ();
        dlg->setFocus ();
        return;
    }

    //-- ищем окно во внешних виджетах
    auto dlgw = _externalDialogs.value (actionName, nullptr);
    if (dlgw != nullptr) {
        unsetCursor ();
        dlgw->setFocus ();
        return;
    }

    if (_mdiArea == nullptr)
    {
        ExternalDialog *dlg = new ExternalDialog (window, this);
        dlg->setWindowFlag (Qt::WindowMaximizeButtonHint, true);
        dlg->setWindowFlag (Qt::WindowMinimizeButtonHint, true);
        dlg->setWindowFlag (Qt::WindowSystemMenuHint, true);
        dlg->setObjectName (actionName);
        dlg->setAttribute (Qt::WA_DeleteOnClose);

        connect (dlg, &ExternalDialog::destroyed,
                 this, &MainWindow::slotExternalDialogDestroyed);
        _externalDialogs.insert (actionName, dlg);
        dlg->show ();
    }
    else
    {
        window->setAccessibleName (actionName);
        window->setAccessibleDescription (SUBWINDOW_TYPE_ADD_IN_MDI);

        QMdiSubWindow* buffWindow = ((QMdiArea*)centralWidget ())->addSubWindow (window);
        buffWindow->setProperty (ACTION_PROP_SIGNATURE, actionName); // <--
        buffWindow->setAccessibleName (actionName);
        buffWindow->setAccessibleDescription (SUBWINDOW_TYPE_ADD_IN_MDI);
        buffWindow->resize (window->size ());
        buffWindow->setMinimumSize (window->minimumSize ());
        buffWindow->setMaximumSize (window->maximumSize ());
        buffWindow->setWindowIcon (window->windowIcon ());

        connect (window, &QWidget::destroyed, buffWindow, &MainWindow::close);
        connect (buffWindow, &QMdiSubWindow::destroyed, this, &MainWindow::slotCloseSubWindow);

        // --- set filter ---
        MdiSubWindowFilter *buffFilter = new MdiSubWindowFilter (buffWindow, window);
        connect (buffFilter, &MdiSubWindowFilter::mousePos, this, &MainWindow::mousePos);
        window->setMouseTracking (true);
        window->installEventFilter (buffFilter);
        // ---

        window->setParent (buffWindow);
        window->show ();

        emit subWindowOpened (buffWindow->accessibleName ());
    }

}

ISystemGuiCorePopUpElement* MainWindow::addPopUpMessage (const QString &messageCaption,
                                                         const QString &messageText,
                                                         ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType messageType,
                                                         bool autoClose,
                                                         const QIcon &messageIcon_16x16,
                                                         const QIcon &messageIcon_48x48)
{
    QIcon icon_16x16;
    QIcon icon_48x48;

    switch(messageType) {
    case ISystemGuiCorePopUpElement::ISystemGuiCorePopUpInfo: {
        icon_16x16 = QIcon(":/icons/icons/information-white.png");
        icon_48x48 = QIcon(":/icons/icons/information_6199.png");
        break;
    }
    case ISystemGuiCorePopUpElement::ISystemGuiCorePopUpOk: {
        icon_16x16 = QIcon(":/icons/icons/tick-circle.png");
        icon_48x48 = QIcon(":/icons/icons/check_9806.png");
        break;
    }
    case ISystemGuiCorePopUpElement::ISystemGuiCorePopUpWarning: {
        icon_16x16 = QIcon(":/icons/icons/exclamation.png");
        icon_48x48 = QIcon(":/icons/icons/alert_2136.png");
        break;
    }
    case ISystemGuiCorePopUpElement::ISystemGuiCorePopUpCritical: {
        icon_16x16 = QIcon(":/icons/icons/icon_cancel.png");
        icon_48x48 = QIcon(":/icons/icons/delete_2626.png");
        break;
    }
    case ISystemGuiCorePopUpElement::ISystemGuiCorePopUpNotType: {
        icon_16x16 = messageIcon_16x16;
        icon_48x48 = messageIcon_48x48;
        break;
    }
    }

    QDateTime addTime = QDateTime::currentDateTimeUtc ();
    SystemGuiCorePopUpElement* buffElement = new SystemGuiCorePopUpElement (QUuid::createUuid ().toString (QUuid::WithoutBraces),
                                                                            messageCaption,
                                                                            messageText,
                                                                            messageType,
                                                                            addTime,
                                                                            icon_16x16,
                                                                            icon_48x48);

    if (!_statusBarInfoWidget) {
        delete buffElement;
        buffElement = nullptr;
        return nullptr;
    }

    if (_isShowed)
        _statusBarInfoWidget->addStatusElement (buffElement, autoClose);
    else
        _popUps.push_back (new PopUpContainer (autoClose, buffElement));

    return buffElement;
}

QWidget *MainWindow::getMainWindowParentWidget ()
{

    return this;
}

IMainWindowConnector *MainWindow::getMainWindowConnector ()
{

    return _mainWindowConnector;
}

