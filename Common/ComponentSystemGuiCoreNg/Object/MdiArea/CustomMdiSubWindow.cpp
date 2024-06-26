#include"CustomMdiSubWindow.h"
#include<QHideEvent>

CustomMdiSubWindow::CustomMdiSubWindow(QWidget *parent) : QMdiSubWindow(parent) {}

void CustomMdiSubWindow::TabHide()
{
    emit HideSubWindow();
}

void CustomMdiSubWindow::hideEvent( QHideEvent * event )
{
    if (windowState() & Qt::WindowMinimized)
    {
        event->ignore();
        emit HideSubWindow();
    }
}

void CustomMdiSubWindow::closeEvent( QCloseEvent * event )
{
    event->ignore();
    emit CloseSubWindow();
}

void CustomMdiSubWindow::resizeEvent(QResizeEvent *event)
{
    QMdiSubWindow::resizeEvent(event);
    widget()->setMaximumSize(this->size());
    widget()->resize(size());
}


