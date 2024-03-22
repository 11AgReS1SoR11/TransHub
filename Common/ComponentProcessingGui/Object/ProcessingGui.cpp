#include "ProcessingGui.h"
#include <ComponentsCore5alpha/ComponentManager>
#include "Object/MapWidget.h"
#include "Object/CommandPanel.h"

#include <QtWidgets/QDockWidget>

ProcessingGui::ProcessingGui (QObject* parent)
    : QObject (parent)
{
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
        auto cdock = new QDockWidget (tr ("Commands"));
        cdock->setFeatures (QDockWidget::DockWidgetMovable
                            | QDockWidget::DockWidgetFloatable
                            /*| QDockWidget::DockWidgetVerticalTitleBar*/);
        cdock->setWidget (new CommandPanel (mwnd));
        mwnd->addDockWidget (Qt::RightDockWidgetArea, cdock);

        mwnd->resizeDocks ({cdock}, {0}, Qt::Horizontal);
        mwnd->resizeDocks ({cdock}, {400}, Qt::Horizontal);
    }

    //-- меню "Планирование"
    auto menu_planning = wnd->getMenu (tr ("Planning"));
    if (menu_planning)
    {
        auto act_commands = new QAction (tr ("Planning"), this);
        menu_planning->addMenuAction (act_commands);
        wnd->addMenuInMenuBar (menu_planning);

        //_actProcessing = new QAction (tr ("Request processing"), this);
        //repairmenu->addMenuAction (_actProcessing);
        //wnd->addMenuInMenuBar (repairmenu);

        //        auto actRoadMap = new QAction (tr ("Request timeline"), this);
        //        repairmenu->addMenuAction (actRoadMap);
        //        wnd->addMenuInMenuBar (repairmenu);
    }
    else
    {
        qWarning () << "[ProcessingGui][initGui] Could not find menu:"
                    << tr ("Planning");
    }
}

QWidget *ProcessingGui::getWidget (const QString &actionName, const QString &actionSignature,
                                       ISystemGuiCoreParentWidget::WidgetType &type,
                                       ISystemGuiCoreParentWidget::WidgetShowType &showType)
{
    (void)actionSignature;
    (void)type;
    (void)showType;

    ISystemGuiCoreMainWindow *wnd = mainWindow ();
    if (!wnd) {
        qCritical () << "[ProcessingGui][getWidget] Could not find main window *";
        return nullptr;
    }

    if (actionName == tr ("Commands"))
    {
        auto fmw = new MapWidget (wnd->getMainWindowParentWidget ());
        fmw->setAttribute (Qt::WA_DeleteOnClose);
        return fmw;
    }

    return nullptr;
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
