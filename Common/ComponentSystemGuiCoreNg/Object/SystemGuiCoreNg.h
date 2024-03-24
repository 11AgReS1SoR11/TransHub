//!
//! \file SystemGuiCoreNg.h
//!

#ifndef SYSTEMGUICORENG_H
#define SYSTEMGUICORENG_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QMutex>
#include <QtCore/QDebug>

#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QMenu>

#include <IObject/ISystemGuiCore/ISystemGuiCore.h>

#include "MainWindow.h"
#include "SystemIconManager.h"

#include "HelpDesigner/HelpDesigner.h"

//!
//! \class SystemGuiCoreNgю.
//! \brief Базовый класс графического ядра системы.
//! \author Кромачев Максим
//!
class SystemGuiCoreNg : public ISystemGuiCore
{
    Q_OBJECT

    friend class MainWindow;

public:
    //! \brief Конструктор класса
    //! \param parent Родительский объект
    explicit SystemGuiCoreNg (QObject *parent = nullptr);

    //! Деструктор класса
    virtual ~SystemGuiCoreNg ();

    //! \brief Инициализация компонента
    //! \param configObject Объект для работы с конфигурационным файлом
    //! \return true / false
    bool initComponent (/*IConfigObject* configObject*/);

    // - ISystemGuiCore

    //! \brief Запросить список сформированных основных окон.
    //! \return Список основных окон
    QVector<ISystemGuiCoreMainWindow*> getMainWindows () const override;

    //! \brief Запросить основное окно по типо отображаемых данных.
    //!        Если необходимое окно отсутствует, возвращается первое сформированное окно
    //!        или окно, отвечающее за отображение всех типов данных.
    //! \param windowType Тип отображаемых данных, поддерживаемое окном
    //! \return Основное окно
    ISystemGuiCoreMainWindow *getMainWindow (const QString &windowType) override;

    //! \brief Зарегистрировать графический компонент.
    //! \param guiComponent Графический компонент
    void registrateGuiComponent (ISystemGuiCoreParentWidget *guiComponent) override;

    //! \brief Инициализация графического интерфейса.
    //! \param administratorMode Режим работы
    //! \param userLogin Логин текущего пользователя
    Q_INVOKABLE void initGui (int userMode, const QString &userLogin);

    //! \brief Регистрация менеджера иконок.
    //!        В системе присутствует всегда только один менеджер иконок!
    //! \param iconManager Менеджер иконок
    void registrateIconManager (ISystemIconManager *iconManager) override;

    //! \brief Регистрация менеджера делегатов для элементов.
    //! \param delegateManager Менеджер делегатов
    void registrateDelegateManager (IDelegateManager* delegateManager) override;

    //! \brief Запросить менеджера иконок.
    //! \return Менеджер иконок
    ISystemIconManager* getIconManager () override;

    //! \brief Запросить конструктор файла справки
    //! \return Конструктор файла справки
    IHelpDesigner* getHelpDesigner () override;

    //! \brief Запросить логин текущего пользователя.
    //! \return Логин пользователя
    QString getCurrentUserLogin () const override;

    //! \brief Вернуть режим работы текущего пользователя
    //! \return Режим работы текущего пользователя
    UserMode userMode () override;

    //! \brief Вернуть объект настроек компонента.
    //! \return Объект настроек компонента.
    ISystemGuiCoreSettings *settings () override;

public slots:
    //! \brief Завершение работы
    Q_INVOKABLE void exitFromSystem ();

    //! \brief Завершение работы
    Q_INVOKABLE void exitWithRequest (const QString &text);

    //! \brief Закрытие отдельного MainWindow
    void destroyMainWindow ();

    //! \brief Активация Action-а в меню иконки системного трея
    void trayIconActionActive ();

protected:
    //! \brief Создание нового MainWindow
    //! \param mainWindowTypes Доступные типы отображаемых данных
    //! \param windowTitle Заголовок окна
    void addMainWindow (const QVector<QString> &mainWindowTypes, const QString &windowTitle) override;

    //! \brief Инициализация графического ядра и всех графических компонентов
    //! \param userMode Режим работы текущего пользователя
    //! \param userLogin Логин текущего пользователя
    void initGuiProtected (UserMode userMode, const QString &userLogin);

    //! \brief Инициализация базовой справки
    void initDefaultHelp ();

    //! \brief Запросить список менеджеров делегатов
    //! \return список менеджеров
    QVector <IDelegateManager*> getDelegateManagers () const;

    //! \brief Вернуть объект системного трея
    //! \return Объект системного трея
    std::shared_ptr<QSystemTrayIcon> systemTrayIcon () override;

private:
    QVector <MainWindow*>                   _mainWindows;        /**< все MainWindow */
    QVector <QString>                       _mainWindowsTypes;   /**< все типы окон */
    QVector <ISystemGuiCoreParentWidget*>   _allGuiComponents;      /**< Все графические компоненты, зарегистрированные в системе */
    QVector <IDelegateManager*>             _allDelegateManagers;   //!< все менеджеры делегатов, зарегистрированные в системе

    ISystemGuiCoreSettings *_settings {nullptr};    //!< Объект настроек.
    ISystemIconManager *_iconManager {nullptr};     //!< Менеджер иконок.
    HelpDesigner*       _helpDesigner {nullptr};    //!< Конструктор файла справки.
    bool                _localIconManager {false};  //!< true - если создан локальный менеджер иконок; false - если был зарегистрирован */

    std::shared_ptr<QSystemTrayIcon> _systemTrayIcon;    //!< иконка приложения в системном трее
    QMenu *_systemTrayIconMenu {nullptr};

    QString _systemName;            //!< Название системы.
    QString _currentUserLogin;      //!< Логин текущего пользователя.
    UserMode _userMode {USER_MODE}; //!< Режим работы текущего пользователя.
    bool _isInit {false};           //!< Флаг инициализации.
    bool _debug {true};             //!< Режим отладки.
};

#endif // SYSTEMGUICORENG_H
