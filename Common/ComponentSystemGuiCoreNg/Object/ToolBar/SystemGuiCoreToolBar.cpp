#include "SystemGuiCoreToolBar.h"
#include "../MainWindowDefines.h"
#include <QtCore/QDebug>

SystemGuiCoreToolBar::SystemGuiCoreToolBar (const QString &title, QWidget *parent)
    : QToolBar (title, parent)
{
    _title = title;
}

SystemGuiCoreToolBar::~SystemGuiCoreToolBar ()
{}

QString SystemGuiCoreToolBar::getTitle () const {
    return _title;
}

QAction *SystemGuiCoreToolBar::addToolBarAction (QAction *toolBarAction,
                                                 ConnectType connectType)
{
    QMutexLocker lock (&_mutex);

    if (!toolBarAction) {
        qCritical () << "[SystemGuiCoreToolBar][addToolBarAction] QAction is NULL! Add action failed!";
        return nullptr;
    }

    for (auto const &value : actions ())
    {
        if (value->text () == toolBarAction->text ())
        {
            qCritical () << QString ("[SystemGuiCoreToolBar][addToolBarAction] Action \"%1\" already inserted in toolbar \"%2\"! Add action failed!")
                            .arg (toolBarAction->text(), _title);
            return nullptr;
        }
    }

    if (connectType == ISystemGuiCoreToolBar::UseConnect && toolBarAction->parent () == nullptr)
    {
        qCritical () << QString ("[SystemGuiCoreToolBar][addToolBarAction] Action \"%1\" (toolbar \"%2\") parent is NULL! Set default connect failed! Add action failed!")
                       .arg (toolBarAction->text (), _title);
        return nullptr;
    }
    //    if(connectType == ISystemGuiCoreToolBar::UseConnect
    //       && action->parent() != nullptr) {
    //        qCritical() << QString("SystemGuiCoreToolBar::addToolBarAction(): Action \"%1\" (toolbar \"%2\") use default connect, but his parent is not NULL! Possible double window initialization!").arg(action->text()).arg(_title);
    //    }

    QString cType = ACTION_PROP_TYPE_USE_CONNECT;
    if (connectType == ISystemGuiCoreToolBar::NotUseConnect)
        cType = ACTION_PROP_TYPE_NOT_USE_CONNECT;

    toolBarAction->setProperty (ACTION_PROP_TYPE, cType);

    addAction (toolBarAction);
    return toolBarAction;
}

QAction *SystemGuiCoreToolBar::addToolBarWidget (QWidget *widget)
{
    QMutexLocker lock(&_mutex);
    return addWidget (widget);
}

void SystemGuiCoreToolBar::clear () {
    QToolBar::clear ();
}

void SystemGuiCoreToolBar::setToolButtonStyle (Qt::ToolButtonStyle toolButtonStyle) {
    QToolBar::setToolButtonStyle (toolButtonStyle);
}

QAction *SystemGuiCoreToolBar::addToolBarAction (const QString &actionName,
                                                 const QIcon &actionIcon,
                                                 const QString &toolTip,
                                                 QObject *parent,
                                                 ConnectType connectType)
{
    QMutexLocker lock(&_mutex);

    if(actionName.isEmpty()) {
        qCritical() << QString("[SystemGuiCoreToolBar][addToolBarAction] Action name is Empty (toolbar \"%1\")! Add action failed!").arg(_title);
        return nullptr;
    }

    QList<QAction*> buffList = this->actions();
    for(int i = 0; i < buffList.size(); i++) {
        QAction* buffAction = buffList[i];
        if(buffAction->text() == actionName) {
            qCritical() << QString("[SystemGuiCoreToolBar][addToolBarAction] Action \"%1\" already inserted in toolbar \"%2\"! Add action failed!")
                           .arg(actionName)
                           .arg(_title);
            return nullptr;
        }
    }

    if(connectType == ISystemGuiCoreToolBar::UseConnect
            && parent == nullptr) {
        qCritical() << QString("[SystemGuiCoreToolBar][addToolBarAction] Action \"%1\" (toolbar \"%2\") parent is NULL! Set default connect failed! Add action failed!")
                       .arg(actionName)
                       .arg(_title);
        return nullptr;
    }

    QString cType = ACTION_PROP_TYPE_USE_CONNECT;
    if (connectType == ISystemGuiCoreToolBar::NotUseConnect)
        cType = ACTION_PROP_TYPE_NOT_USE_CONNECT;

    QAction* buffAction = new QAction (actionIcon, actionName, parent);
    buffAction->setProperty (ACTION_PROP_TYPE, cType);

    if (toolTip.isEmpty () == false)
        buffAction->setToolTip (toolTip);

    addAction (buffAction);
    return buffAction;
}
