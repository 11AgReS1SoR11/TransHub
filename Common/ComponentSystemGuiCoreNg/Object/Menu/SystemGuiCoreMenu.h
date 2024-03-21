#ifndef SYSTEMGUICOREMENU_H
#define SYSTEMGUICOREMENU_H

#include <QtCore/QMutex>
#include <QtWidgets/QMenu>

#include <IObject/ISystemGuiCore/ISystemGuiCoreMenu.h>

//!
//! \file SystemGuiCoreMenu.h
//! \class SystemGuiCoreMenu.
//! \brief Базовый класс меню.
//! \author Ivanov Anton, Gorbachev Ivan
//!
class SystemGuiCoreMenu : public QMenu, public ISystemGuiCoreMenu
{
    Q_OBJECT

public:   
    //! \brief Конструктор класса.
    //! \param title Заголовок.
    //! \param parent Родительский виджет.
    explicit SystemGuiCoreMenu (const QString &title, QWidget *parent = nullptr);

    //! \brief Деструктор класса.
    virtual ~SystemGuiCoreMenu ();

    //! \brief Отметить меню как добавленное в MenuBar
    //! \param value true / false
    void addedInMenuBar (bool value);

    //! \brief Отметить, что меню является подменю
    //! \param value true / false
    void setSubMenu (bool value);

    //! \brief Добавлено ли в MenuBar
    //! \return true / false
    bool isAdded () const;

    //! \brief Является ли подменю
    //! \return true / false
    bool isSubMenu () const;

    //! \brief Поиск QAction-а по его названию
    //! \param actionName Название QAction-а
    //! \return QAction
    QAction* getAction (const QString& actionName) const;

    //! \brief Запросить название меню
    //! \return Название меню
    QString getMenuName () const override;

    //! \brief Добавить подменю
    //! \param subMenu Меню
    void addSubMenu (ISystemGuiCoreMenu *subMenu) override;

    //! \brief Вернуть подменю.
    //! \param name Название подменю.
    //! \return Подменю.
    ISystemGuiCoreMenu *subMenu (const QString &name) override;

    //! \brief Добавить в меню разделитель
    void addMenuSeparator () override;

    //! \brief Добавить QAction в меню
    //! \param actionName Название
    //! \param actionIcon Иконка
    //! \param actionParent Родитель (если используется тип соединения UseConnect,
    //! то родитель должен быть ISystemGuiCoreParentWidget!)
    //! \param connectType Тип соединения
    //! \return QAction
    QAction *addMenuAction (const QString &actionName, const QIcon &actionIcon,
                            QObject *actionParent = nullptr,
                            ConnectType connectType = UseConnect) override;

    //! \brief Добавить QAction в меню
    //! \param action QAction
    //! \param connectType Тип соединения
    //! \return QAction
    QAction *addMenuAction (QAction *action, ConnectType connectType = UseConnect) override;

    //! \brief Установить состояние видимости меню
    //! \param value true/false
    void setMenuVisible (bool value) override;

    //! \brief Проверка состояния видимости меню
    //! \return trye/false
    bool isMenuVisible () const override;

    //! \brief Установить тип сортировки меню.
    //! \param value Тип сортировки.
    //! \author Gorbachev Ivan
    void setOrderType (OrderType value) override;

    //! \brief Вернуть тип сортировки.
    //! \return Тип сортировки.
    //! \author Gorbachev Ivan
    OrderType orderType () const override;

protected:
    //! \brief Добавить пункт меню в определённом порядке.
    //! \param action Пункт меню.
    //! \author Gorbachev Ivan
    void addWithOrger (QAction *action);

    QAction *_lastAddedAction {nullptr};    //!< последний добавленный QAction
    bool _addedInMenuBar {false};           //!< добавлено ли в менюбар
    bool _isSubMenu {false};                //!< является ли подменю
    OrderType _order {Alphabet};            //!< Тип сортировки.
};

#endif // SYSTEMGUICOREMENU_H
