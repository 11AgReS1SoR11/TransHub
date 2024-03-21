#include "SystemGuiCoreMenu.h"
#include "../MainWindowDefines.h"
#include <QtCore/QDebug>

SystemGuiCoreMenu::SystemGuiCoreMenu (const QString &title, QWidget *parent)
    : QMenu (title, parent)
{}

SystemGuiCoreMenu::~SystemGuiCoreMenu () {}

void SystemGuiCoreMenu::addedInMenuBar (bool value) {
    _addedInMenuBar = value;
}

void SystemGuiCoreMenu::setSubMenu (bool value) {
    _isSubMenu = value;
}

bool SystemGuiCoreMenu::isAdded () const {
    return _addedInMenuBar;
}

bool SystemGuiCoreMenu::isSubMenu () const {
    return _isSubMenu;
}

QString SystemGuiCoreMenu::getMenuName () const {
    return title ();
}

void SystemGuiCoreMenu::addSubMenu (ISystemGuiCoreMenu *subMenu)
{
    if (!subMenu) {
        qCritical() << "[SystemGuiCoreMenu][addSubMenu] Submenu is NULL! Add failed!";
        return;
    }

    auto buffSubMenu = dynamic_cast<SystemGuiCoreMenu*> (subMenu);
    if (!buffSubMenu) {
        qCritical() << "[SystemGuiCoreMenu][addSubMenu] Convert to SystemGuiCoreMenu failed! Add failed!";
        return;
    }

    if (buffSubMenu->isAdded ())
        return;
    if (buffSubMenu->isSubMenu ())
        return;

    if (!_lastAddedAction)
        _lastAddedAction = addMenu (buffSubMenu);
    else
        _lastAddedAction = insertMenu (_lastAddedAction, buffSubMenu);

    buffSubMenu->setSubMenu (true);
}

ISystemGuiCoreMenu *SystemGuiCoreMenu::subMenu (const QString &name)
{
    if (name.isEmpty ())
        return nullptr;

    auto list = actions ();
    for (QAction *action : qAsConst (list))
    {
        if (action->isSeparator ())
            continue;

        if (action->menu ())
        {
            auto submenu = dynamic_cast<ISystemGuiCoreMenu *> (action->menu ());
            if (submenu && getMenuName () == name)
                return submenu;
        }
    }

    return nullptr;
}

void SystemGuiCoreMenu::addMenuSeparator () {
    addSeparator ();
}

QAction* SystemGuiCoreMenu::getAction (const QString &actionName) const
{
    QList<QAction*> list = actions ();
    for (QAction *action : qAsConst (list))
    {
        if (action->text () == actionName)
            return action;
    }

    return nullptr;
}

QAction* SystemGuiCoreMenu::addMenuAction (const QString &actionName, const QIcon &actionIcon,
                                           QObject *actionParent, ISystemGuiCoreMenu::ConnectType connectType)
{
    auto list = actions ();
    for (QAction *action : qAsConst (list))
    {
        if (action->text () == actionName)
        {
            qCritical () << "[SystemGuiCoreMenu][addMenuAction] Action" << actionName
                         << "already inserted in menu" << title () << "Add action failed!";
            return nullptr;
        }
    }

    if (connectType == ISystemGuiCoreMenu::UseConnect && actionParent == nullptr)
    {
        qWarning () << "[SystemGuiCoreMenu][addMenuAction] Action" << actionName
                    << "parent is NULL! Set default connect failed! Add action failed!";
        return nullptr;
    }

    auto buffAction = new QAction (actionIcon, actionName, actionParent);
    if (!_lastAddedAction)
        _lastAddedAction = buffAction;

    QString cType = ACTION_PROP_TYPE_USE_CONNECT;
    if (connectType == ISystemGuiCoreMenu::NotUseConnect)
        cType = ACTION_PROP_TYPE_NOT_USE_CONNECT;

    buffAction->setProperty (ACTION_PROP_TYPE, cType);
    buffAction->setIconVisibleInMenu (true);

    addWithOrger (buffAction);
    return buffAction;
}

QAction* SystemGuiCoreMenu::addMenuAction (QAction *action, ConnectType connectType)
{
    if (!action) {
        qWarning () << "[SystemGuiCoreMenu][addMenuAction] QAction is null! Add action failed!";
        return nullptr;
    }
    if (action->text ().isEmpty ()) {
        qWarning () << "[SystemGuiCoreMenu][addMenuAction] QAction text is empty! Add action failed!";
        return nullptr;
    }

    auto list = actions ();
    for (QAction *act : qAsConst (list))
    {
        if (act->text () == action->text ()) {
            qCritical() << "[SystemGuiCoreMenu][addMenuAction] Action" << action->text ()
                        << "already inserted in menu" << title () << "! Add action failed!";
            return nullptr;
        }
    }

    if (connectType == ISystemGuiCoreMenu::UseConnect && action->parent () == nullptr)
    {
        qWarning () << "[SystemGuiCoreMenu][addMenuAction] Action" << action->text ()
                    << "parent is NULL! Set default connect failed! Add action failed!";
        return nullptr;
    }

    if (!_lastAddedAction)
        _lastAddedAction = action;

    QString cType = ACTION_PROP_TYPE_USE_CONNECT;
    if (connectType == ISystemGuiCoreMenu::NotUseConnect)
        cType = ACTION_PROP_TYPE_NOT_USE_CONNECT;

    action->setProperty (ACTION_PROP_TYPE, cType);
    action->setIconVisibleInMenu (true);

    addWithOrger (action);
    return action;
}

void SystemGuiCoreMenu::setMenuVisible (bool value) {
    setVisible (value);
}

bool SystemGuiCoreMenu::isMenuVisible () const {
    return isVisible ();
}

void SystemGuiCoreMenu::setOrderType (ISystemGuiCoreMenu::OrderType value)
{
    if (_order == value)
        return;
    if (value == Origin)
        qWarning () << "[SystemGuiCoreMenu][setSortType] UNDER CONSTRUCTION";
    else if (value == Alphabet)
        qWarning () << "[SystemGuiCoreMenu][setSortType] UNDER CONSTRUCTION";
    else if (value == Priority)
        qWarning () << "[SystemGuiCoreMenu][setSortType] UNDER CONSTRUCTION";
    else if (value == GroupAlphabet)
        qWarning () << "[SystemGuiCoreMenu][setSortType] UNDER CONSTRUCTION";
    else if (value == GroupPriority)
        qWarning () << "[SystemGuiCoreMenu][setSortType] UNDER CONSTRUCTION";
}

ISystemGuiCoreMenu::OrderType SystemGuiCoreMenu::orderType () const {
    return _order;
}

void SystemGuiCoreMenu::addWithOrger (QAction *action)
{
    QList<QAction*> lst = actions ();

    if (_order == Alphabet) {
        //-- если пусто
        if (lst.isEmpty ()) {
            addAction (action);
            return;
        }
        //-- ищем в алфавитном порядке
        for (int i = 0; i < lst.size (); ++i)
        {
            if (lst[i]->text () >= action->text ()) {
                insertAction (lst[i], action);
                return;
            }
        }
        //-- не нашли, добавляем в конец
        addAction (action);
    }
    else {
        //-- в остальных случаях просто добавляем
        addAction (action);
    }
}
