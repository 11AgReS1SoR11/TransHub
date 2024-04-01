#include"CustomMdiSubWindow.h"
#include<QHideEvent>

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
