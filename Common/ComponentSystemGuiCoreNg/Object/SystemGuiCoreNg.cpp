#include "SystemGuiCoreNg.h"
#include "GuiCoreTagDefines.h"
#include "SystemGuiCoreSettings.h"

#include <QtWidgets/QMessageBox>
#include <Application/IApplication.h>

SystemGuiCoreNg::SystemGuiCoreNg (QObject *parent)
    : ISystemGuiCore (parent)
{
    _helpDesigner = new HelpDesigner (this);
    initDefaultHelp ();
}

SystemGuiCoreNg::~SystemGuiCoreNg ()
{
    //-- если менеджер иконок поумолчанию
    if (_localIconManager && _iconManager) {
        SystemIconManager *im = dynamic_cast<SystemIconManager*> (_iconManager);
        if (im) delete im;
        _iconManager = nullptr;
    }

    if (_systemTrayIconMenu)
        delete _systemTrayIconMenu;
    _systemTrayIconMenu = nullptr;

    if (_helpDesigner)
        delete _helpDesigner;

    _allDelegateManagers.clear();
    _allGuiComponents.clear();
    _mainWindowsTypes.clear();
    _mainWindows.clear();
    _currentUserLogin.clear();
}

QVector<IDelegateManager *> SystemGuiCoreNg::getDelegateManagers () const {
    return _allDelegateManagers;
}

std::shared_ptr<QSystemTrayIcon> SystemGuiCoreNg::systemTrayIcon() {
    return _systemTrayIcon;
}

bool SystemGuiCoreNg::initComponent (/*IConfigObject *configObject*/)
{
    _systemName = tr ("Graphic Core System \"Phoenix\"");

    QVector<QString> temp_types;
    QString displayName;


     // add window
    if(displayName.isEmpty()) {
        if(_mainWindows.isEmpty())
            displayName = _systemName;
        else
            displayName = QString("%1 %2").arg(_systemName).arg(_mainWindows.size());
    }

    temp_types.push_back(QString("*"));
    addMainWindow (temp_types, displayName);

    if (_mainWindows.size() == 0) {
        qCritical() << qPrintable(tr("[SystemGuiCoreNg][initComponent] Не добавлено ни одного окна! Инициализация остановлена! Смотри конфигурационный файл!"));
        return false;
    }

    return true;
}

void SystemGuiCoreNg::addMainWindow (const QVector<QString> &mainWindowTypes, const QString &windowTitle)
{
    QVector <QString> tempTypes;

    //-- проверяем нет ли окна с тем же свойством
    for (int i = 0; i < mainWindowTypes.size(); i++) {
        if (!_mainWindowsTypes.contains(mainWindowTypes[i]))
            tempTypes.push_back(mainWindowTypes[i]);
    }

    //-- если отредактированный список свойств нового окна не пуст - создаем окно
    if (tempTypes.size () == 0)
        return;

    auto window = new MainWindow ();
    window->addWindowTypes (tempTypes);
    window->setMainWindowTitle (windowTitle);
    window->setWindowIcon (getIconManager ()->getSystemLogo (16, 16));

    QObject::connect (window, &MainWindow::mainWindowClosed, this, &SystemGuiCoreNg::destroyMainWindow);
    connect (window, &MainWindow::exitFromSystem, this, &SystemGuiCoreNg::exitFromSystem);

    _mainWindows.push_back (window);
    for (int i = 0; i < tempTypes.size (); i++)
        _mainWindowsTypes.push_back (tempTypes[i]);

    if (_debug)
        qDebug () << "[SystemGuiCoreNg][addMainWindow] Added a new main window:"
                  << window->windowTitle ();
}

QVector<ISystemGuiCoreMainWindow*> SystemGuiCoreNg::getMainWindows () const
{
    QVector <ISystemGuiCoreMainWindow*> tempVector;
    for (int i = 0; i < _mainWindows.size(); i++)
        tempVector.push_back (_mainWindows[i]);

    return tempVector;
}

ISystemGuiCoreMainWindow* SystemGuiCoreNg::getMainWindow (const QString &windowType)
{
    if (_mainWindows.size () == 0) {
        qCritical () << "[SystemGuiCoreNg][getMainWindow] No one main window found";
        return nullptr;
    }

    // ищем окно с нужным типом
    MainWindow *allTypes = nullptr;
    for (int i = 0; i < _mainWindows.size (); i++) {
        if (_mainWindows[i]->containsType ("*"))
            allTypes = _mainWindows[i];

        if (_mainWindows[i]->containsType (windowType))
            return _mainWindows[i];
    }
    // если не нашли
    if (allTypes)
        return allTypes;

    allTypes = _mainWindows[0];
    allTypes->addWindowType ("*");
    return allTypes;
}

void SystemGuiCoreNg::registrateGuiComponent (ISystemGuiCoreParentWidget* guiComponent)
{
    if (!guiComponent) {
        qCritical () << "[SystemGuiCoreNg][registrateGuiComponent] ISystemGuiCoreParentWidget is NULL! Registrate failed!";
        return;
    }
    _allGuiComponents.push_back (guiComponent);
}

void SystemGuiCoreNg::initGui (int userMode, const QString &userLogin)
{
    if (_isInit)
        return;

    emit startInitGui ();
    initGuiProtected (static_cast<UserMode> (userMode), userLogin);
}

void SystemGuiCoreNg::registrateIconManager (ISystemIconManager *iconManager)
{
    if (!iconManager) {
        qCritical() << "[SystemGuiCoreNg][registrateIconManager] ISystemIconManager is NULL! Registrate failed!";
        return;
    }

    _localIconManager = false;
    _iconManager = iconManager;
}

void SystemGuiCoreNg::registrateDelegateManager (IDelegateManager *delegateManager)
{
    if (!delegateManager) {
        qCritical() << "[SystemGuiCoreNg][registrateDelegateManager] IDelegateManager is NULL! Registrate failed!";
        return;
    }
    _allDelegateManagers.push_back (delegateManager);
}

ISystemIconManager* SystemGuiCoreNg::getIconManager ()
{
    if (!_iconManager) {
        _localIconManager = true;
        _iconManager = new SystemIconManager ();
    }
    return _iconManager;
}

IHelpDesigner *SystemGuiCoreNg::getHelpDesigner () {
    return _helpDesigner;
}

QString SystemGuiCoreNg::getCurrentUserLogin () const {
    return _currentUserLogin;
}

ISystemGuiCore::UserMode SystemGuiCoreNg::userMode () {
    return _userMode;
}

ISystemGuiCoreSettings *SystemGuiCoreNg::settings ()
{
    if (!_settings)
        _settings = new SystemGuiCoreSettings (this);
    return _settings;
}

void SystemGuiCoreNg::initGuiProtected (UserMode userMode, const QString &userLogin)
{
    //-- добавляем trayIcon
    if (!_systemTrayIcon) {
        _systemTrayIcon = std::make_shared<QSystemTrayIcon>
                (getIconManager ()->getSystemLogo (QSize (16,16)), this);
        _systemTrayIcon->setToolTip (_systemName);
        _systemTrayIcon->setVisible (false);

        _systemTrayIconMenu = new QMenu ();
        _systemTrayIcon->setContextMenu (_systemTrayIconMenu);
    }

    _userMode = userMode;
    _currentUserLogin = userLogin;

    // заполняем окна необходимыми объектами
    for (int i = 0; i < _mainWindows.size (); i++)
    {
        MainWindow* buffWindow = _mainWindows[i];

        //-- добавляем раздел в меню
        QAction* appAction = _systemTrayIcon->contextMenu ()->addAction (QIcon (":/icons/icons/application.png"),
                                                                         buffWindow->windowTitle ());
        appAction->setEnabled (false);
        appAction->setIconVisibleInMenu (true);
        connect (appAction, &QAction::triggered, this, &SystemGuiCoreNg::trayIconActionActive);

        if (i == 0)
            buffWindow->setUseAppLoaderSettings(true);

        buffWindow->setSystemGuiCore (this);
        buffWindow->setHelpDesigner (_helpDesigner);
        buffWindow->setSystemName (_systemName);
        buffWindow->setWindowIcon (getIconManager ()->getSystemLogo (QSize (16,16)));
        buffWindow->setIconManager (getIconManager ());
        buffWindow->setSystemTrayIcon (_systemTrayIcon);
        buffWindow->setUserMode (_userMode);
        buffWindow->setUserLogin (_currentUserLogin);
    }

    //-- добавляем раздел "выйти" в trayIcon
    _systemTrayIcon->contextMenu ()->addSeparator ();
    QAction* exitAction = _systemTrayIcon->contextMenu ()->addAction (
                QIcon (":/icons/icons/door-open-out.png"), tr ("Выйти из системы"));
    exitAction->setEnabled (true);
    exitAction->setWhatsThis ("exit");
    exitAction->setIconVisibleInMenu (true);
    connect (exitAction, &QAction::triggered, this, &SystemGuiCoreNg::trayIconActionActive);
    _systemTrayIcon->setVisible (true);

    //-- инициализируем все графические компоненты
    for (ISystemGuiCoreParentWidget *parentwidget : qAsConst (_allGuiComponents))
        parentwidget->initGui ();

    if (_helpDesigner) {
        _helpDesigner->initHelp ();
        _helpDesigner->setUserMode (_userMode);
        _helpDesigner->setIconManager (getIconManager());
        _helpDesigner->setSystemName (_systemName);
    }

    IApplication *app = dynamic_cast<IApplication*> (qApp);
    if (app) {
        connect (app, &IApplication::applicationQuit, this, &SystemGuiCoreNg::exitFromSystem);
    } else {
        qCritical () << "[SystemGuiCoreNg] Convert to IApplication failed!";
    }

    _isInit = true;
    emit finishInitGui ();
}

void SystemGuiCoreNg::initDefaultHelp ()
{
    if (!_helpDesigner)
        return;

    // add default Gui help data
    IHelpSection* buffSection = _helpDesigner->helpSection(tr("Управление интерфейсом комплекса"));
    if(buffSection) {
        buffSection->setTextFile(new QFile(":/helpFiles/help/files/SettingsGuiCore.txt"));

        // app loader
        IHelpItem *item = new IHelpItem (tr ("Graphics kernel loader"));
        item->setTextFile(new QFile(":/helpFiles/help/files/SettingsAppLoader.txt"));
        item->addImageResource(QString("menu_settings.png"), QImage(":/helpImages/help/images/menu_settings.png"));
        item->addImageResource(QString("settings_app_loader.png"), QImage(":/helpImages/help/images/settings_app_loader.png"));
        item->addImageResource(QString("app_loader_start.png"), QImage(":/helpImages/help/images/app_loader_start.png"));
        item->addImageResource(QString("app_loader_warning_1.png"), QImage(":/helpImages/help/images/app_loader_warning_1.png"));
        item->addImageResource(QString("app_loader_warning_2.png"), QImage(":/helpImages/help/images/app_loader_warning_2.png"));
        item->addImageResource(QString("app_loader_start_settings.png"), QImage(":/helpImages/help/images/app_loader_start_settings.png"));
        item->addImageResource(QString("app_loader_info.png"), QImage(":/helpImages/help/images/app_loader_info.png"));
        buffSection->addItem(item);

        // left panel
        item = new IHelpItem(tr("Левая информационная панель"));
        item->setTextFile(new QFile(":/helpFiles/help/files/SettingsLeftPanel.txt"));
        item->addImageResource(QString("settings_left_panel.png"), QImage(":/helpImages/help/images/settings_left_panel.png"));
        item->addImageResource(QString("left_panel_size.png"), QImage(":/helpImages/help/images/left_panel_size.png"));
        item->addImageResource(QString("left_panel_show.png"), QImage(":/helpImages/help/images/left_panel_show.png"));
        item->addImageResource(QString("left_panel_move_tabs.png"), QImage(":/helpImages/help/images/left_panel_move_tabs.png"));
        item->addImageResource(QString("base_panel_tabs_pos.png"), QImage(":/helpImages/help/images/base_panel_tabs_pos.png"));
        buffSection->addItem(item);

        // right panel
        item = new IHelpItem(tr("Правая информационная панель"));
        item->setTextFile(new QFile(":/helpFiles/help/files/SettingsRightPanel.txt"));
        item->addImageResource(QString("settings_right_panel.png"), QImage(":/helpImages/help/images/settings_right_panel.png"));
        item->addImageResource(QString("right_panel_size.png"), QImage(":/helpImages/help/images/right_panel_size.png"));
        item->addImageResource(QString("right_panel_show.png"), QImage(":/helpImages/help/images/right_panel_show.png"));
        item->addImageResource(QString("right_panel_move_tabs.png"), QImage(":/helpImages/help/images/right_panel_move_tabs.png"));
        buffSection->addItem(item);

        // bottom panel
        item = new IHelpItem(tr("Нижняя информационная панель"));
        item->setTextFile(new QFile(":/helpFiles/help/files/SettingsBottomPanel.txt"));
        item->addImageResource(QString("settings_bottom_panel.png"), QImage(":/helpImages/help/images/settings_bottom_panel.png"));
        item->addImageResource(QString("bottom_panel_size.png"), QImage(":/helpImages/help/images/bottom_panel_size.png"));
        item->addImageResource(QString("bottom_panel_show.png"), QImage(":/helpImages/help/images/bottom_panel_show.png"));
        item->addImageResource(QString("bottom_panel_move_tabs.png"), QImage(":/helpImages/help/images/bottom_panel_move_tabs.png"));
        buffSection->addItem(item);

        // toolbars
        item = new IHelpItem (tr ("Settings toolbars"));
        item->setTextFile(new QFile(":/helpFiles/help/files/SettingsToolBars.txt"));
        item->addImageResource(QString("settings_toolbars.png"), QImage(":/helpImages/help/images/settings_toolbars.png"));
        buffSection->addItem(item);

        // mdi area
        item = new IHelpItem (tr ("Workspace"));
        item->setTextFile(new QFile(":/helpFiles/help/files/SettingsMdiArea.txt"));
        item->addImageResource(QString("settings_mdi_area.png"), QImage(":/helpImages/help/images/settings_mdi_area.png"));
        item->addImageResource(QString("mdi_area_subwindows.png"), QImage(":/helpImages/help/images/mdi_area_subwindows.png"));
        item->addImageResource(QString("mdi_area_tabs.png"), QImage(":/helpImages/help/images/mdi_area_tabs.png"));
        item->addImageResource(QString("mdi_area_close_tab.png"), QImage(":/helpImages/help/images/mdi_area_close_tab.png"));
        buffSection->addItem(item);

        // windows
        item = new IHelpItem(tr("Управление открытыми окнами"));
        item->setTextFile(new QFile(":/helpFiles/help/files/MenuWindows.txt"));
        item->addImageResource(QString("menu_windows.png"), QImage(":/helpImages/help/images/menu_windows.png"));
        item->addImageResource(QString("menu_windows_tile.png"), QImage(":/helpImages/help/images/menu_windows_tile.png"));
        item->addImageResource(QString("menu_windows_cascade.png"), QImage(":/helpImages/help/images/menu_windows_cascade.png"));
        buffSection->addItem(item);

        // hide to tray
        item = new IHelpItem(tr("Сворачивание в системный трей"));
        item->setTextFile(new QFile(":/helpFiles/help/files/MenuHideToTray.txt"));
        item->addImageResource(QString("menu_hide_to_tray.png"), QImage(":/helpImages/help/images/menu_hide_to_tray.png"));
        item->addImageResource(QString("menu_hide_to_tray_msg.png"), QImage(":/helpImages/help/images/menu_hide_to_tray_msg.png"));
        item->addImageResource(QString("menu_hide_to_tray_context_menu.png"), QImage(":/helpImages/help/images/menu_hide_to_tray_context_menu.png"));
        buffSection->addItem(item);

        // exit
        item = new IHelpItem(tr("Завершение работы"));
        item->setTextFile(new QFile(":/helpFiles/help/files/MenuExit.txt"));
        item->addImageResource(QString("menu_exit.png"), QImage(":/helpImages/help/images/menu_exit.png"));
        buffSection->addItem(item);

        _helpDesigner->addSectionInHelp(buffSection);
    }

    buffSection = _helpDesigner->helpSection (tr ("Справочная информация о комплексе"));
    if (buffSection) {
        buffSection->setTextFile(new QFile(":/helpFiles/help/files/GuiCoreInfo.txt"));

        // components state
        IHelpItem *item = new IHelpItem (tr ("Статусы компонентов"));
        item->setTextFile (new QFile (":/helpFiles/help/files/ComponentsState.txt"));
        item->addImageResource(QString ("menu_about_system.png"), QImage(":/helpImages/help/images/menu_about_system.png"));
        item->addImageResource(QString ("components_state.png"), QImage(":/helpImages/help/images/components_state.png"));
        item->addImageResource(QString ("components_state_init_ok.png"), QImage(":/helpImages/help/images/components_state_init_ok.png"));
        item->addImageResource (QString ("components_state_init_err.png"), QImage(":/helpImages/help/images/components_state_init_err.png"));
        item->addImageResource (QString ("components_state_load_err.png"), QImage(":/helpImages/help/images/components_state_load_err.png"));
        buffSection->addItem(item);

        // about system
        item = new IHelpItem (tr ("О системе"));
        item->setTextFile(new QFile(":/helpFiles/help/files/AboutSystem.txt"));
        item->addImageResource(QString("about_system.png"), QImage(":/helpImages/help/images/about_system.png"));
        buffSection->addItem(item);

        // about qt
        item = new IHelpItem (tr ("О Qt"));
        item->setTextFile(new QFile(":/helpFiles/help/files/AboutQt.txt"));
        item->addImageResource(QString("about_qt.png"), QImage(":/helpImages/help/images/about_qt.png"));
        buffSection->addItem(item);

        _helpDesigner->addSectionInHelp(buffSection);
    }

    buffSection = _helpDesigner->helpSection(tr("Уведомления"));
    if(buffSection) {
        buffSection->setTextFile(new QFile(":/helpFiles/help/files/GuiCorePopUps.txt"));

        // popups msg
        IHelpItem *item = new IHelpItem(tr("Управление уведомлениями"));
        item->setTextFile(new QFile(":/helpFiles/help/files/MainWindowPopUpMsg.txt"));
        item->addImageResource(QString("main_window_popup_0.png"), QImage(":/helpImages/help/images/main_window_popup_0.png"));
        item->addImageResource(QString("main_window_popup_1.png"), QImage(":/helpImages/help/images/main_window_popup_1.png"));
        item->addImageResource(QString("main_window_popup_list.png"), QImage(":/helpImages/help/images/main_window_popup_list.png"));
        item->addImageResource(QString("main_window_popup_list_context_menu.png"), QImage(":/helpImages/help/images/main_window_popup_list_context_menu.png"));
        item->addImageResource(QString("main_window_popup_info.png"), QImage(":/helpImages/help/images/main_window_popup_info.png"));
        item->addImageResource(QString("main_window_popup_ok.png"), QImage(":/helpImages/help/images/main_window_popup_ok.png"));
        item->addImageResource(QString("main_window_popup_warning.png"), QImage(":/helpImages/help/images/main_window_popup_warning.png"));
        item->addImageResource(QString("main_window_popup_err.png"), QImage(":/helpImages/help/images/main_window_popup_err.png"));
        buffSection->addItem(item);

        // popups msg sound
        item = new IHelpItem(tr("Управление звуком уведомлений"));
        item->setTextFile(new QFile(":/helpFiles/help/files/MainWindowPopUpMsgSound.txt"));
        item->addImageResource(QString("main_window_popup_sound_0.png"), QImage(":/helpImages/help/images/main_window_popup_sound_0.png"));
        item->addImageResource(QString("main_window_popup_sound_1.png"), QImage(":/helpImages/help/images/main_window_popup_sound_1.png"));
        buffSection->addItem(item);

        _helpDesigner->addSectionInHelp(buffSection);
    }
}

void SystemGuiCoreNg::destroyMainWindow ()
{
    MainWindow* buffWindow = qobject_cast<MainWindow*> (sender());
    if (!buffWindow)
        return;

    QVector<QString> types = buffWindow->windowTypes ();
    for (int i = 0; i < types.size(); i++)
        _mainWindowsTypes.remove (_mainWindowsTypes.indexOf (types[i]));

    buffWindow->clearAllMainWindow ();
    _mainWindows.remove (_mainWindows.indexOf(buffWindow));
    buffWindow->deleteLater ();

    // проверяем есть ли открытые окна, если нет, разворачиваем первое в списке
    bool visibleWindows = false;
    for (int i = 0; i < _mainWindows.size(); i++) {
        if(_mainWindows[i]->isVisible())
            visibleWindows = true;
    }

    if (!visibleWindows && _mainWindows.size() > 0) {
        _mainWindows[0]->setVisible(true);
        _mainWindows[0]->activateWindow();
    }

    if (_mainWindows.size () == 0) {
        delete _helpDesigner;
        _helpDesigner = nullptr;

        exitFromSystem ();
    }
}

void SystemGuiCoreNg::trayIconActionActive()
{
    QAction* buffAction = qobject_cast<QAction*> (sender());
    if (buffAction->text () == tr ("Выйти из системы")) {
        exitFromSystem ();
        return;
    }

    for (int i = 0; i < _mainWindows.size (); i++) {
        if (_mainWindows[i]->mainWindowTitle () != buffAction->text ()) {
            continue;
        } else {
            if(!_mainWindows[i]->isVisible ()) {
                _mainWindows[i]->setVisible (true);
                _mainWindows[i]->activateWindow ();
                return;
            } else {
                _mainWindows[i]->activateWindow ();
                return;
            }
        }
    }
}

void SystemGuiCoreNg::exitFromSystem ()
{
    for (auto const &mw : qAsConst (_mainWindows)) {
        mw->closeMainWindowTray ();
        mw->close ();
    }

    QApplication::quit ();
}

void SystemGuiCoreNg::exitWithRequest (const QString &text)
{
    auto wnd = getMainWindow ("*");

    QMessageBox msgBox (wnd != nullptr ? wnd->getMainWindowParentWidget () : nullptr);
    msgBox.setIcon (QMessageBox::Question);
    msgBox.setText (text);
    msgBox.setStandardButtons (QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton (QMessageBox::Cancel);
    int ret = msgBox.exec ();

    switch (ret) {
    case QMessageBox::Ok:
        exitFromSystem ();
        break;
    case QMessageBox::Cancel:
        return;
    }
}
