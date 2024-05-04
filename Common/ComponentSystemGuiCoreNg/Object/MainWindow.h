//!
//! \file MainWindow.h
//!

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <IObject/ISystemGuiCore/ISystemGuiCoreParentWidget.h>
#include <IObject/ISystemGuiCore/ISystemGuiCoreMainWindow.h>
#include <IObject/ISystemGuiCore/ISystemIconManager.h>

#include <QtCore/QSignalMapper>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QDebug>

#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMdiSubWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QSystemTrayIcon>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLayout>

#include "MainWindowConnector.h"

#include "MdiArea/MdiAreaFilter.h"
#include "MdiArea/MdiSubWindowFilter.h"

#include "MainWindowConsole/MainWindowConsole.h"

#include "WindowCreator/SystemWindowsCreator.h"

#include "BasePanel/BasePanelDockWidget.h"
#include "BasePanel/BasePanelWidget.h"

#include "Menu/SystemGuiCoreMenu.h"

#include "ToolBar/SystemGuiCoreToolBar.h"

#include "StatusBar/StatusBarFilter.h"

#include "StatusBarInfo/StatusBarInfoWidget.h"
#include "StatusBarInfo/SystemGuiCorePopUpElement.h"

#include "StatusBarVolume/StatusBarVolume.h"

#include "SettingsPanel/SettingsPanelContainer.h"
#include "SettingsPanel/SettingsBasePanel.h"
#include "SettingsPanel/SettingsToolBar.h"
#include "SettingsPanel/SettingsGuiCoreWidget.h"
#include "SettingsPanel/SettingsAppLoader.h"

#include "StatusBar/StatusBarTabContainer.h"

#include "HelpDesigner/HelpDesigner.h"

#include "ExternalMainWindow.h"
#include "ExternalDialog.h"

class SystemGuiCoreNg;
class PopUpContainer;

namespace Ui {
class MainWindow;
}

//!
//! \class MainWindow.
//! \brief Класс главного окна.
//! \author Кромачев Максим
//!
class MainWindow : public QMainWindow, public ISystemGuiCoreMainWindow
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param parent Родительский виджет
    explicit MainWindow (QWidget *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~MainWindow ();

    //! \brief Установить центральный виджет
    //! \param value Виджет
    //! \return true/false
    //!
    //! Метод может вернуть false, если уже тругой компонент
    //! установил центральный виджет в это окно.
    bool initCentralWidget (QWidget *value) override;

    //! \brief Проверка наличия центрального виджета.
    //! \return true/false
    bool isCentralWidget () override;

    //! \brief Удаление всех объектов MainWindow
    void clearAllMainWindow ();

    //! \brief Обработка активации окна
    //! \param event QEvent
    void changeEvent (QEvent *event) override;

    //! \brief Обработка закрытия окна
    //! \param event QCloseEvent
    void closeEvent (QCloseEvent *event) override;

    //! \brief Задать иконку системного трея
    //! \param systemTrayIcon QSystemTrayIcon
    void setSystemTrayIcon (const std::shared_ptr<QSystemTrayIcon> &systemTrayIcon);

    //! \brief Задать менеджер иконок
    //! \param iconManager ISystemIconManager
    void setIconManager (ISystemIconManager* iconManager);

    //! \brief Сбросит временное значение сворачивания в трей и разрешит закрыть окно
    void closeMainWindowTray ();

    //! \brief Добавить поддерживаемый тип отображаемых данных окна
    //! \param windowType Тип
    void addWindowType (const QString &windowType);

    //! \brief Добавить поддерживаемые типы отображаемых данных окна
    //! \param windowTypes Вектор типов
    void addWindowTypes (const QVector<QString> &windowTypes);

    //! \brief Проверка поддержки типа
    //! \param windowType Тип окна
    //! \return true / false
    bool containsType (const QString &windowType);

    //! \brief Запросить вектор поддерживаемых типов
    //! \return Вектор типов
    QVector<QString> windowTypes ();

    //! \brief Задать заголовок окна
    //! \param windowTitle Заголовок окна
    void setMainWindowTitle (const QString &windowTitle);

    //! \brief Запросить заголовок окна
    //! \return Заголовок окна
    QString mainWindowTitle ();

    //! \brief Задать системное имя
    //! \param systemName Системное имя
    void setSystemName (const QString &systemName);

    //! \brief Задать режим работы - администратор
    //! \param value true / false
    void setUserMode (ISystemGuiCore::UserMode userMode);

    //! \brief Задать логин текущего пользователя
    //! \param userLogin Логин
    void setUserLogin (const QString &userLogin);

    //! \brief Задать ядро графической системы
    //! \param guiCore SystemGuiCoreNg
    void setSystemGuiCore (SystemGuiCoreNg* guiCore);

    //! \brief Добавить в раздел настроек панель для настройки загрузчика
    //! \param isUse true / false
    void setUseAppLoaderSettings (bool isUse);

    //! \brief Задать конструктор файла справки
    //! \param helpDesigner Конструктор файла справки
    void setHelpDesigner (HelpDesigner *helpDesigner);

    //! \brief Отобразить/Скпыть меню.
    //! \param value true / false
    void setMenuVisible (bool value) override;

    //! \brief Проверка видимости меню.
    //! \return Меню.
    bool isMenuVisible () const override;

    //! \brief Запросить меню.
    //! \param menuName Название меню
    //! \return Меню
    ISystemGuiCoreMenu* getMenu (const QString& menuName) override;

    //! \brief Добавить меню в меню бар.
    //! \param menu Меню
    bool addMenuInMenuBar (ISystemGuiCoreMenu* menu) override;

    //! \brief Запросить панель инструментов.
    //! \param toolBarName Название панели
    //! \return Панель инструментов
    ISystemGuiCoreToolBar* getToolBar (const QString &toolBarName) override;

    //! \brief Запросить информационную панель.
    //! \param panelType Тип панели
    //! \return Базовая панель
    ISystemGuiCoreBasePanel* getPanel (ISystemGuiCoreBasePanel::BasePanelType panelType) override;

    //! \brief Запросить статус бар.
    //! \return Статус бар
    QStatusBar* getStatusBar () override;

    //! \brief Добавить виджет настроек.
    //! \param widget Виджет настроек.
    //! \return true / false;
    bool addSettingsWidget (ISystemGuiCoreSettingsPanelWidget *widget) override;

    //! \brief Удалить виджет настроек
    //! \param widget Виджет
    //! \return true / false;
    bool removeSettingsWidget (ISystemGuiCoreSettingsPanelWidget* widget) override;

    //! \brief Добавить закладку.
    //! \param tabWidget Закладка.
    //! \return true / false.
    bool addTabWidget( ISystemGuiCoreStatusBarTabWidget * tabWidget ) override;

    //! \brief Задать активное окно внутри рабочей области.
    //! \param windowName Название окна (название QAction-а, которое его вызвало)
    //! \return true / false - если MainWindow не содержит запрашиваемое окно
    bool mdiSubWindowSetFocus (const QString& windowName) override;

    //! \brief Закрыть окно внутри рабочей области.
    //! \param window Название окна (название QAction-а, которое его вызвало)
    //! \return true / false - если MainWindow не содержит запрашиваемое окно
    bool mdiSubWindowClose (QWidget* window) override;

    //! \brief Добавить новое окно на рабочую область.
    //!        Метод выполнится только в том случае, если добавляемое окно отсутствует на рабочей области
    //! \param actionName Название окна (название QAction-а, которое его вызвало)
    //! \param window Виджет окна
    void addWindowInMdiArea (const QString& actionName, QWidget* window) override;

    //! \brief Добавить всплывающее сообщение.
    //! \param messageCaption Заголовок сообщения
    //! \param messageText Текст сообщения
    //! \param messageType Тип сообщения
    //! \param autoClose Автоматически скрывать
    //! \param messageIcon_16x16 Иконка сообщения 16x16 px
    //! \param messageIcon_48x48 Иконка сообщения 48x48 px
    //! \return Объект всплывающего сообщения
    ISystemGuiCorePopUpElement* addPopUpMessage (const QString& messageCaption,
                                                 const QString& messageText,
                                                 ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType messageType = ISystemGuiCorePopUpElement::ISystemGuiCorePopUpNotType,
                                                 bool autoClose = true,
                                                 const QIcon &messageIcon_16x16 = QIcon(),
                                                 const QIcon &messageIcon_48x48 = QIcon()) override;

    //! \brief Запросить родительский виджет.
    //!        Указатель на MainWindow
    //! \return Родительский виджет
    QWidget* getMainWindowParentWidget () override;

    //! \brief Запросить коннектор MainWindow
    //! \return Коннектор MainWindow
    IMainWindowConnector* getMainWindowConnector () override;

    //! \brief Запросить режим работы.
    //! \return false / true - если приложение в режиме супер пользователя
    int userMode ();

    //! \brief Запросить логин текущего пользователя.
    //! \return Логин пользователя
    QString getUserLogin () const override;

    //! \brief Открыть MainWindow.
    void show () override;

    //! \brief Скрыть MainWindow
    void hide () override;

    //! \brief Проверка, что прижение в режиме MDI
    //! \return true/false
    bool isMdiMode () const override;

private:
    Ui::MainWindow *ui {nullptr}; /**< объект графического интерфейса */

    mutable QMutex _mutex;      /**< мьютекс */

    SettingsPanelContainer  *_settingsPanelContainer {nullptr};   //!< контейнер виджетов настроек
    SystemGuiCoreNg         *_systemGuiCore {nullptr};            //!< ядро GuiCore
    ISystemIconManager      *_iconManager {nullptr};              /**< менеджер иконок */
    MainWindowConnector     *_mainWindowConnector {nullptr};      /**< коннектор MainWindow */
    std::weak_ptr<QSystemTrayIcon> _systemTrayIcon;               //!< иконка системного трея
    SystemWindowsCreator    *_windowsCreator {nullptr};           /**< "создатель" системных окон */
    StatusBarInfoWidget     *_statusBarInfoWidget {nullptr};      /**< виджет информации о всплывающих сообщениях в статус баре */
    StatusBarVolume         *_statusBarVolume {nullptr};          /**< виджет звука в статус баре (вкл./выкл.) */
    QMdiArea                *_mdiArea {nullptr};                  /**< центральный виджет рабочей области */
    MdiAreaFilter           *_mdiAreaFilter {nullptr};            /**< фильтр событий центрального виджета */
    HelpDesigner            *_helpDesigner {nullptr};             /**< конструктор файла справки */
    StatusBarTabContainer   *_statusBarTabContainer {nullptr};    /**< виджет, хранящий вкладки окон */

    // --- подменю окна
    QSignalMapper   *_windowMapper {nullptr};   //!< объект связки сигналов и отправителей
    QMenu           *_windowMenu {nullptr};     //!< Меню.
    bool            _menuVisible {true};        //!< Флаг видимости меню.
    QAction         *_closeAct {nullptr};       //!< закрыть активное
    QAction         *_closeAllAct {nullptr};    //!< закрыть все
    QAction         *_tileAct {nullptr};        //!< расположить плиткой
    QAction         *_cascadeAct {nullptr};     //!< расположить каскадом
    QAction         *_separatorAct {nullptr};   //!< разделитель
    // ---

    QString _systemName;    /**< название всей системы */
    QString _windowTitle;   /**< название главного окна (т.к. при разворачивании дочерних окон название главного меняется!!!) */
    QString _userLogin;     /**< логин текущего пользователя */

    bool _minimizeToTray {false};       //!< сворачивание в трей
    bool _minimizeToTrayTemp {false};   //!< сворачивание в трей (для временного хранения)
    ISystemGuiCore::UserMode _userMode = ISystemGuiCore::USER_MODE; //!< Режим работы текущего пользователя
    bool _useAppLoaderSettings {false}; //!< добавлять ли в раздел настроек панель для настройки загрузчика
    bool _isShowed {false};             //!< открылось ли окно

    QVector <QString> _allWindowTypes; /**< все поддерживаемые типы отображаемых данных в MainWindow */

    QMap<ISystemGuiCoreBasePanel::BasePanelType, BasePanelDockWidget*>  _basePanels;        /**< информационные панели */
    QMap<ISystemGuiCoreBasePanel::BasePanelType, BasePanelWidget*>      _basePanelsWidgets; /**< виджеты информационных панелей */

    QVector<PopUpContainer*> _popUps; //!< список контейнеров всплывающих сообщений

    //! \brief Список внешних окон (QMainWindow)
    //! key - сигнатура QAction
    //! value - окно
    QMap<QString, ExternalMainWindow*> _externalMainWindows;

    //! \brief Список внешних окон (QDialog)
    //! key - сигнатура QAction
    //! value - окно
    QMap<QString, ExternalDialog*> _externalDialogs;

protected:
    //! \brief Инициализация MainWindow
    void init ();

    //! \brief Инициализация панелей инструментов
    void initToolBars ();

    //! \brief Инициализация информационных панелей
    void initPanels ();

    //! \brief Добавить панель
    //! \param panelType Тип панели
    //! \param type Тип QDockWidget
    //! \param widget QWidget
    void addPanel (ISystemGuiCoreBasePanel::BasePanelType panelType,
                   Qt::DockWidgetArea type,
                   BasePanelWidget *widget);

    //! \brief Инициализация базового меню "File"
    void initDefaultMenuFile ();

    //! \brief Инициализация прочих базовых меню
    void initDefaultMenus ();

    //! \brief Инициализация прочих базовых тулбаров
    void initDefaultToolbars ();

    //! \brief Инициализация меню
    void initMenu ();

    //! \brief Сортировка всех меню в алфавитном порядке
    void sortMenuByNames ();

    //! \brief Инициализация обработки событий QAction-ов в меню
    //! Каждому пункту меню сохдаётся connect к слоту \see clickAction
    //! \param parentMenu Родительское меню
    void initMenuConnect (QMenu *parentMenu = nullptr);

    //! \brief Задать подписи QAction-ам в меню.
    //! \param parentMenu Родительское меню.
    //! \param signaturePath Раздел.
    void initMenuSignature (QMenu *parentMenu = nullptr, const QString &signaturePath = QString (""));

    //! \brief Инициализация системных панелей настроек
    void initSettingsPanels ();

    //! \brief Загрузка сохраненных ранее параметров
    void loadWindowStyle ();

    //! \brief Сохранение настройки окна перед закрытием
    void saveWindowStyle ();

    //! \brief Загрузка открытого ранее окна, добавленного с помощью метода "void addWindowInMdiArea(...)"
    //! \param windowName Название QAction-а, который открыл это окно
    void loadAddedMdiAreaWindow (const QString &windowName);

    //! \brief Загрузка открытого ранее окна, добавленного с помощью метода "void addWindowInMdiArea(...)"
    //! \param windowName Название QAction-а, который открыл это окно
    //! \param panel Тип панели
    //! \return true / false
    bool loadAddedWindow (const QString& windowName, ISystemGuiCoreBasePanel::BasePanelType panel);

    //! \brief Поиск меню в MenuBar
    //! \param menuName Название меню
    //! \param parentMenu Родительское меню
    //! \return QMenu
    QMenu *findMenu (const QString& menuName, QMenu* parentMenu = nullptr);

    //! \brief Поиск панели инструментов
    //! \param toolbarName Название панели инструментов
    //! \return QToolBar
    QToolBar *findToolbar (const QString& toolbarName);

    //! \brief Поиск QAction-а в MenuBar
    //! \param actionName Название QAction-а
    //! \param parentMenu Родительское меню
    //! \return QAction
    QAction *findMenuAction (const QString& actionName, QMenu* parentMenu = nullptr);

    //! \brief Поиск QAction-а в MenuBar по его подписе
    //! \param signaturePath Подпись
    //! \param parentMenu Родительское меню
    //! \return QAction
    QAction *findMenuActionSignature (const QString& signaturePath, QMenu* parentMenu = nullptr);

    //! \brief Поиск QAction-а на панелях инструментов
    //! \param actionName Название QAction-а
    //! \return QAction
    QAction *findToolbarsAction (const QString& actionName);

    //! \brief Поиск QAction-а на панелях инструментов по его подписе
    //! \param signaturePath Подпись
    //! \return QAction
    QAction *findToolbarsActionSignature (const QString &signaturePath);

    //! \brief Поиск окна в рабочей области MDI
    //! \param windowName Подпись окна
    //! \return QMdiSubWindow
    QMdiSubWindow *findMdiSubWindow (const QString &windowName);

    //! \brief Запрос QAction-а из меню иконки системного трея, отвечающего за данное MainWindow
    //! \return QAction
    QAction *getTrayIconAction ();

    //! \brief Количество окон, открытых из QAction-ов меню или панелей инструментов
    //! \return int
    int getNumActionWindows ();

    //! \brief Количество окон, добавленных с помощью метода "void addWindowInMdiArea(...)"
    //! \return int
    int getNumAddInMdiWindows ();

    //! \brief Очистить список контейнеров всплывающих сообщений
    void clearPopUps ();

public slots:
    //! \brief Обновление меню "Окна"
    void updateWindowMenu ();

    //! \brief Обновление меню в зависимости от активного окна
    void updateMenus ();

private slots:
    //! \brief Отобразить все всплывающие сообщения
    void showPopUps ();

    //! \brief Открытие окна с информацией о Qt
    void clickAboutQt ();

    //! \brief Обработка кликов всех QAction-ов
    void clickAction ();

    //! \brief Обработка состояния QAction-ов
    //! \param value true / false
    void checkedAction (bool value);

    //! \brief Активация окна
    //! \param window Виджет окна
    void setActiveSubWindow (QWidget *window);

    //! \brief Обработка закрытия внутреннего окна
    void slotCloseSubWindow ();

    //! \brief Обработчик удаления внешнего окна.
    //!
    //! Все внешние окна (QDialog) создаются с флагом
    //! Qt::WA_DeleteOnClose, поэтому при закрытии
    //! будут автоматически удалены.
    //!
    //! \param object Объект окна.
    void slotExternalDialogDestroyed (QObject *object);

    //! \brief Обработчик удаления внешнего окна.
    //!
    //! Все внешние окна (QMainWindow) создаются с флагом
    //! Qt::WA_DeleteOnClose, поэтому при закрытии
    //! будут автоматически удалены.
    //!
    //! \param object Объект окна.
    void slotExternalMainWindowDestroyed (QObject *object);

signals:
    //! \brief Сигнал о закрытии MainWindow
    void mainWindowClosed ();

    //! \brief Сигнал о выходе из системы и завершении работы
    void exitFromSystem ();

    //! \brief Сигнал об открытии окна внутри рабочей области.
    //! \param windowName Название окна (подпись QAction-а, которое его вызвало)
    //!        Пример подписи: Инструменты/Окна/Закрыть активное
    //!        т.е. Инструменты - меню в MenuBar-е
    //!             Окна - подменю
    //!             Закрыть активное - название QAction-а
    //!
    //!        Если QAction распологается на панели инструментов, то перед подписью будет префикс TOOLBAR
    //!        Пример: TOOLBAR Окна/Закрыть активное
    //!        т.е. Окна - название панели инструментов
    //!             Закрыть активное - название QAction-а
    void subWindowOpened (const QString &windowName);

    //! \brief Сигнал о закрытии окна внутри рабочей области.
    //! \param windowName Название окна (подпись QAction-а, которое его вызвало)
    //!        Пример подписи: Инструменты/Окна/Закрыть активное
    //!        т.е. Инструменты - меню в MenuBar-е
    //!             Окна - подменю
    //!             Закрыть активное - название QAction-а
    //!
    //!        Если QAction распологается на панели инструментов, то перед подписью будет префикс TOOLBAR
    //!        Пример: TOOLBAR Окна/Закрыть активное
    //!        т.е. Окна - название панели инструментов
    //!             Закрыть активное - название QAction-а
    void subWindowClosed (const QString &windowName);

    //! \brief Сигнал о положении курсора мыши
    //! \param pos Положение курсора
    void mousePos (const QPoint &pos);

    //! \brief Сигнал о положении курсора мыши над панелью состояния
    void mouseStatusBarEnter ();
};

//!
//! \file MainWindow.h
//! \class PopUpContainer
//! \brief Класс контейнера всплывающих сообщений
//!
class PopUpContainer : public QObject
{
public:
    //! \brief Конструктор
    //! \param parent Родительский объект
    explicit PopUpContainer (QObject *parent = nullptr)
        : QObject (parent)
        , _autoClose (true)
        , _popUp (nullptr)
    {}

    //! \brief Конструктор
    //! \param autoClose Автоматически скрывать
    //! \param popUp Объект сообщения
    //! \param parent Родительский объект
    PopUpContainer (bool autoClose,
                    SystemGuiCorePopUpElement *popUp,
                    QObject *parent = nullptr)
        : QObject (parent)
        , _autoClose (autoClose)
        , _popUp (popUp)
    {}

    //! \brief Деструктор
    virtual ~PopUpContainer () {
        _popUp = nullptr;
    }

    bool _autoClose {true};                         //!< Автоматически скрывать.
    SystemGuiCorePopUpElement *_popUp {nullptr};    //!< Объект сообщения.
};

#endif // MAINWINDOW_H
