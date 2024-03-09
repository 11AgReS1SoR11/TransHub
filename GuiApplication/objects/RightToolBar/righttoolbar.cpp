#include "righttoolbar.h"


RightToolBar::RightToolBar(QToolBar *parent): QToolBar(parent)
{


}

RightToolBar::RightToolBar(const QString &title, QWidget *parent): QToolBar(title, parent)
{

    build_route = new QAction(tr("GO"), this);
    build_route->setStatusTip(tr("Building a route"));
    //connect(newFileAction, &QAction::triggered, this, &TransHubWindow::newFile);

    stop_route = new QAction(tr("STOP"), this);
    stop_route->setStatusTip(tr("Stop the calculation"));
    //connect(openFileAction, &QAction::triggered, this, &TransHubWindow::openFile);

    setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
    addAction(build_route);
    addAction(stop_route);
}

RightToolBar::~RightToolBar()
{

}
