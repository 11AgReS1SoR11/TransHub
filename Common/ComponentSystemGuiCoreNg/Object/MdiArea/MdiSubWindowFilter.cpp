#include "MdiSubWindowFilter.h"
#include <QtCore/QFlags>
#include <QtCore/QDebug>

MdiSubWindowFilter::MdiSubWindowFilter (QMdiSubWindow *subWindow, QObject *parent)
    : QObject (parent)
    ,_subWindow (subWindow)
{}

MdiSubWindowFilter::~MdiSubWindowFilter () {
    _subWindow = nullptr;
}

void MdiSubWindowFilter::OnDeleteMdiSubWindow()
{
    _subWindow = nullptr;
    deleteLater();
}

bool MdiSubWindowFilter::eventFilter (QObject *obj, QEvent *event)
{
    if ( _subWindow )
    {
        if (event->type () == QEvent::MouseMove
           && _subWindow->windowState ().testFlag (Qt::WindowActive)
           && _subWindow->windowState ().testFlag (Qt::WindowMaximized))
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*> (event);
            if (mouseEvent->button () == Qt::NoButton) {
                emit mousePos (mouseEvent->pos ());
            }
        }
    }

    return QObject::eventFilter (obj, event);
}
