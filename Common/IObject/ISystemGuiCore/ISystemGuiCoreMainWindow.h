#ifndef ISYSTEMGUICOREMAINWINDOW_H
#define ISYSTEMGUICOREMAINWINDOW_H

#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QWidget>

#include "ISystemGuiCoreMenu.h"
#include "ISystemGuiCoreBasePanel.h"
#include "ISystemGuiCoreToolBar.h"
#include "ISystemGuiCorePopUpElement.h"
#include "ISystemGuiCoreStyleItemDelegate.h"
#include "ISystemGuiCoreSettingsPanelWidget.h"
#include "ISystemGuiCoreStatusBarTabWidget.h"
#include "IMainWindowConnector.h"

//!
//! \file ISystemGuiCoreMainWindow.h
//! \class ISystemGuiCoreMainWindow
//! \brief Базовый класс MainWindow
//! \author Иванов Антон, Горбачёв Иван
//!
class ISystemGuiCoreMainWindow
{
public:
    //! \brief Деструктор
    virtual ~ISystemGuiCoreMainWindow () {}

    //! \brief Установить центральный виджет
    //! \param value Виджет
    //! \return true/false
    //! Метод возвращает false, если центральный виджет уже установлен.
    virtual bool initCentralWidget (QWidget *value) = 0;

    //! \brief Проверка наличия центрального виджета.
    //! \return true/false
    virtual bool isCentralWidget () = 0;

    //! \brief Запросить меню.
    //! \param menuName Название меню
    //! \return Меню
    virtual ISystemGuiCoreMenu *getMenu (const QString &menuName) = 0;

    //! \brief Добавить меню в меню бар.
    //! \param menu Меню
    //! \return true/false
    virtual bool addMenuInMenuBar (ISystemGuiCoreMenu *menu) = 0;

    //! \brief Запросить панель инструментов.
    //! \param toolBarName Название панели
    //! \return Панель инструментов
    virtual ISystemGuiCoreToolBar *getToolBar (const QString &toolBarName) = 0;

    //! \brief Запросить информационную панель.
    //! \param panelType Тип панели
    //! \return Базовая панель
    virtual ISystemGuiCoreBasePanel *getPanel (ISystemGuiCoreBasePanel::BasePanelType panelType) = 0;

    //! \brief Запросить статус бар.
    //! \return Статус бар.
    virtual QStatusBar *getStatusBar () = 0;

    //! \brief Добавить виджет настроек.
    //! \param widget Виджет.
    //! \return true / false.
    virtual bool addSettingsWidget (ISystemGuiCoreSettingsPanelWidget *widget) = 0;

    //! \brief Удалить виджет настроек.
    //! \param widget Виджет.
    //! \return true / false.
    virtual bool removeSettingsWidget (ISystemGuiCoreSettingsPanelWidget *widget) = 0;

    //! \brief Добавить закладку.
    //! \param tabWidget Закладка.
    //! \return true / false.
    virtual bool addTabWidget( ISystemGuiCoreStatusBarTabWidget * tabWidget ) = 0;

    //! \brief Задать активное окно внутри рабочей области.
    //! \param windowName Название окна (название QAction-а, которое его вызвало)
    //! \return true / false - если MainWindow не содержит запрашиваемое окно
    virtual bool mdiSubWindowSetFocus (const QString &windowName) = 0;

    //! \brief Закрыть окно внутри рабочей области.
    //! \param window Виджет окна
    //! \return true / false - если MainWindow не содержит запрашиваемое окно
    virtual bool mdiSubWindowClose (QWidget *window) = 0;

    //! \brief Добавить новое окно на рабочую область.
    //!        Метод выполнится только в том случае, если добавляемое окно отсутствует на рабочей области
    //! \param actionName Название окна (название QAction-а, которое его вызвало)
    //! \param window Виджет окна
    virtual void addWindowInMdiArea (const QString &actionName, QWidget *window) = 0;

    //! \brief Добавить всплывающее сообщение.
    //! \param messageCaption Заголовок сообщения
    //! \param messageText Текст сообщения
    //! \param messageType Тип сообщения
    //! \param autoClose Автоматически скрывать
    //! \param messageIcon_16x16 Иконка сообщения 16x16 px
    //! \param messageIcon_48x48 Иконка сообщения 48x48 px
    //! \return Объект всплывающего сообщения
    virtual ISystemGuiCorePopUpElement* addPopUpMessage (const QString& messageCaption,
                                                         const QString& messageText,
                                                         ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType messageType = ISystemGuiCorePopUpElement::ISystemGuiCorePopUpNotType,
                                                         bool autoClose = true,
                                                         const QIcon &messageIcon_16x16 = QIcon(),
                                                         const QIcon &messageIcon_48x48 = QIcon()) = 0;

    //! \brief Запросить родительский виджет.
    //!        Указатель на MainWindow
    //! \return Родительский виджет
    virtual QWidget* getMainWindowParentWidget () = 0;

    //! \brief Запросить коннектор MainWindow
    //! \return Коннектор MainWindow
    virtual IMainWindowConnector* getMainWindowConnector () = 0;

    //! \brief Запросить логин текущего пользователя.
    //! \return Логин пользователя
    virtual QString getUserLogin () const = 0;

    //! \brief Проверка, что прижение в режиме MDI
    //! \return true/false
    virtual bool isMdiMode () const = 0;

    //! \brief Отобразить/Скпыть меню.
    //! \param value true / false
    virtual void setMenuVisible (bool value) = 0;

    //! \brief Проверка видимости меню.
    //! \return Меню.
    virtual bool isMenuVisible () const = 0;

    // ------- НЕ ИСПОЛЬЗОВАТЬ! ------------------------------------------------------

    //! \brief Открыть MainWindow.
    //!
    //! Использование данного метода компонентами категорически запрещено!
    virtual void show () = 0;

    //! \brief Скрыть MainWindow
    virtual void hide () = 0;
};

#endif // ISYSTEMGUICOREMAINWINDOW_H
