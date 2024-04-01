#include "ProcessingGui.h"
#include <ComponentsCore5alpha/ComponentManager>
#include "Object/CommandPanel.h"
#include "../ComponentSystemGuiCoreNg/Object/MainWindowDefines.h"
#include "GuiWidgetsManager.h"

#include <QtWidgets/QDockWidget>
#include "../ComponentSystemGuiCoreNg/Object/MdiArea/CustomMdiSubWindow.h"
#include <QMdiSubWindow>

ProcessingGui::ProcessingGui (QObject* parent)
    : QObject (parent)
{
    _gwmanager = new GuiWidgetsManager( this );
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
        auto cdock = new QDockWidget (tr ("Routes"));
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
                                   WidgetType &, WidgetShowType & )
{
    ISystemGuiCoreMainWindow *wnd = mainWindow ();
    if (!wnd)
    {
        qCritical () << "[ProcessingGui][getWidget] Could not find main window *";
        return nullptr;
    }


    /*Custom*/QMdiSubWindow * mdisw = new QMdiSubWindow/*CustomMdiSubWindow*/;
    QWidget * gwgt = _gwmanager->GetWidget( actionName );
    gwgt->setParent( wnd->getMainWindowParentWidget() );
    mdisw->setWidget( gwgt );

    auto gw_sett = _gwmanager->GetWidgetSettings( actionName );
    if ( gw_sett != std::experimental::nullopt )
        mdisw->restoreGeometry( gw_sett.value() );

//    connect( mdisw, &CustomMdiSubWindow::SaveGWSettings, _gwmanager, &GuiWidgetsManager::OnSaveGWSettings );
//    connect( mdisw, &CustomMdiSubWindow::CloseGWidget, _gwmanager, &GuiWidgetsManager::OnCloseGWidget );

    return mdisw;
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
