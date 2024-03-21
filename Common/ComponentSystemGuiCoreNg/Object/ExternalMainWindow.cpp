#include "ExternalMainWindow.h"

ExternalMainWindow::ExternalMainWindow (QWidget *childWidget, QWidget *parent)
    : QMainWindow (parent)
    , _childWidget (childWidget)
{
    if (_childWidget) {
        _childWidget->setParent (this);
        setWindowTitle (_childWidget->windowTitle ());
        setCentralWidget (_childWidget);
    }
}

//void ExternalMainWindow::closeEvent (QCloseEvent *event)
//{
//    Q_UNUSED (event)
//    emit finished (0);
//}

