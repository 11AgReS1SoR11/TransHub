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
