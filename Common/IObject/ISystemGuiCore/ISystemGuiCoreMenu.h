#ifndef ISYSTEMGUICOREMENU_H
#define ISYSTEMGUICOREMENU_H

#include <QtCore/QString>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>

//!
//! \file ISystemGuiCoreMenu.h
//! \class ISystemGuiCoreMenu
//! \brief Базовый класс меню
//!
class ISystemGuiCoreMenu
{
protected:
    ISystemGuiCoreMenu () {}
    //! \brief Деструктор класса
    virtual ~ISystemGuiCoreMenu () {}

public:
    //! \brief Переичсление типов соединений для добавляемых QAction-ов
    enum ConnectType {
        UseConnect = 0, //!< Будет использован дефолтный коннект в ISystemGuiCoreMainWindow
        NotUseConnect   //!< Отслеживание изменения состояния QAction-а возложена на разработчика компонента
    };

    //! \brief Тип сортировки меню.
    //! \author Горбачёв Иван
    enum OrderType {
        Origin = 0,     //!< В порядке очереди.
        Alphabet,       //!< В алфавитном порядке.
        Priority,       //!< По приоритету.
        GroupAlphabet,  //!< В алфавитном порядке с учётом групп.
        GroupPriority,  //!< По приоритету порядке с учётом групп.
    };

    //! \brief Запросить название меню
    //! \return Название меню
    virtual QString getMenuName () const = 0;

    //! \brief Добавить подменю
    //! \param subMenu Меню
    virtual void addSubMenu (ISystemGuiCoreMenu *subMenu) = 0;

    //! \brief Вернуть подменю.
    //! \param name Название подменю.
    //! \return Подменю.
    virtual ISystemGuiCoreMenu *subMenu (const QString &name) = 0;

    //! \brief Добавить в меню разделитель
    virtual void addMenuSeparator () = 0;

    //! \brief Добавить QAction в меню
    //! \param actionName Название
    //! \param actionIcon Иконка
    //! \param actionParent Родительский объект
    //! Если используется тип соединения UseConnect, то родитель должен быть ISystemGuiCoreParentWidget!
    //! \param connectType Тип соединения
    //! \return QAction
    virtual QAction *addMenuAction (const QString &actionName, const QIcon &actionIcon,
                                    QObject *actionParent = nullptr,
                                    ConnectType connectType = UseConnect) = 0;

    //! \brief Добавить QAction в меню
    //! \param action Добавляемый QAction
    //! \param connectType Тип соединения
    //! \return Добавляемый QAction. В случае ошибки nullptr
    virtual QAction *addMenuAction (QAction *action, ConnectType connectType = UseConnect) = 0;

    //! \brief Установить состояние видимости меню
    //! \param value true/false
    virtual void setMenuVisible (bool value) = 0;

    //! \brief Проверка состояния видимости меню
    //! \return trye/false
    virtual bool isMenuVisible () const = 0;

    //! \brief Установить тип сортировки меню.
    //! \param value Тип сортировки.
    //! \author Горбачёв Иван
    virtual void setOrderType (OrderType value) = 0;

    //! \brief Вернуть тип сортировки.
    //! \return Тип сортировки.
    //! \author Горбачёв Иван
    virtual OrderType orderType () const = 0;
};

#endif // ISYSTEMGUICOREMENU_H
