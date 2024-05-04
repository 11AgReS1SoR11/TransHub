#include <QtWidgets/QDockWidget>
#include <QMdiSubWindow>

#include <ComponentsCore5alpha/ComponentManager>

#include "ProcessingGui.h"
#include "Object/CommandPanel.h"
#include "ProcessingGuiTabWindowsManager.h"

#include "ComponentSystemGuiCoreNg/Object/MainWindowDefines.h"
#include "ISystemGuiCoreStatusBarTabWidget.h"
#include "ISystemGuiCoreStatusBarTabWindow.h"
#include "StatusBarMapWidget.h"
#include "StatusBarPlanningWidget.h"

ProcessingGui::ProcessingGui (QObject* parent)
    : QObject (parent)
{
    _gwmanager = new ProcessingGuiTabWindowsManager( this );
    if (auto gc = guicore ())
        gc->registrateGuiComponent (this);
    else
        qCritical () << "[ProcessingGui][ProcessingGui] Could not find 'ISystemGuiCore' object";
}

void ProcessingGui::initGui ()
{
    //-- главное окно
    auto wnd = mainWindow ();
    if (!wnd) {
        qCritical () << "[ProcessingGui][initGui] Could not find main window *";
        return;
    }


    auto mwnd = dynamic_cast<QMainWindow*> (wnd->getMainWindowParentWidget ());
    if (mwnd)
    {
        auto cdock = new QDockWidget ();
        cdock->setFeatures (QDockWidget::DockWidgetMovable
                            | QDockWidget::DockWidgetFloatable
                            /*| QDockWidget::DockWidgetVerticalTitleBar*/);
        cdock->setWidget (new CommandPanel (mwnd));
        mwnd->addDockWidget (Qt::LeftDockWidgetArea, cdock);

        mwnd->resizeDocks ({cdock}, {0}, Qt::Horizontal);
        mwnd->resizeDocks ({cdock}, {400}, Qt::Horizontal);
        mwnd->setContentsMargins(0, 0, 0, 0);
    }

    //-- меню "Планирование"
    auto menu_planning = wnd->getMenu (tr ("Planning"));
    if (menu_planning)
    {
        QAction * act_commands = new QAction (tr ("Planning"), this);
        menu_planning->addMenuAction (act_commands);
        act_commands->setProperty(ACTION_SHOW_TYPE, true);
        wnd->addMenuInMenuBar (menu_planning);
    }
    else
    {
        qWarning () << "[ProcessingGui][initGui] Could not find menu:"
                    << tr ("Planning");
    }

    //-- меню "Карта"
    auto menu_map = wnd->getMenu (tr ("Map"));
    if (menu_map)
    {
        QAction * act_commands = new QAction (tr ("Map"), this);
        act_commands->setProperty(ACTION_SHOW_TYPE, true);
        menu_map->addMenuAction (act_commands);
        wnd->addMenuInMenuBar (menu_map);
    }
    else
    {
        qWarning () << "[ProcessingGui][initGui] Could not find menu:"
                    << tr ("Map");
    }
}

QWidget *ProcessingGui::getWidget ( const QString & actionName, const QString &,
                                   WidgetType &, WidgetShowType & showType )
{
    ISystemGuiCoreMainWindow *wnd = mainWindow ();
    if (!wnd)
    {
        qCritical () << "[ProcessingGui][getWidget] Could not find main window *";
        return nullptr;
    }

    if ( _gwmanager->Contained( actionName ) )
        return nullptr;

    ISystemGuiCoreStatusBarTabWindow * tabw = _gwmanager->GetWidget( actionName );
    tabw->setParent( wnd->getMainWindowParentWidget() );

    connect( tabw, &ISystemGuiCoreStatusBarTabWindow::SaveTabWindowSettings,
             _gwmanager, &ProcessingGuiTabWindowsManager::OnSaveGWSettings );
    connect( tabw, &ISystemGuiCoreStatusBarTabWindow::CloseTabWindow,
             _gwmanager, &ProcessingGuiTabWindowsManager::OnCloseGWidget );

    ISystemGuiCoreStatusBarTabWidget * sbtw;
    if ( actionName == tr("Планирование") )
        sbtw = new StatusBarPlanningWidget( (QMainWindow *)wnd->getMainWindowParentWidget(), tabw->GetWindowIcon() );
    else if ( actionName == tr("Карта") )
        sbtw = new StatusBarMapWidget( (QMainWindow *)wnd->getMainWindowParentWidget(), tabw->GetWindowIcon() );

    sbtw->ConnectTabWindow( tabw );
    wnd->addTabWidget( sbtw );

    return tabw;
}

ISystemGuiCore *ProcessingGui::guicore () const
{
    using namespace ComponentsCore;
    return ComponentManager::getObject<ISystemGuiCore> ();
}

ISystemGuiCoreMainWindow *ProcessingGui::mainWindow () const
{
    using namespace ComponentsCore;
    auto gc = ComponentManager::getObject<ISystemGuiCore> ();
    if (!gc)
        return nullptr;
    return gc->getMainWindow ("*");
}
