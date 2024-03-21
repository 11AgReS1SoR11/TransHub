#include "StatusBarFilter.h"
#include <QtCore/QDebug>

StatusBarFilter::StatusBarFilter (QObject *parent)
    : QObject (parent)
{}

StatusBarFilter::~StatusBarFilter ()
{}

bool StatusBarFilter::eventFilter (QObject *obj, QEvent *event)
{
    if (event->type () == QEvent::Enter)
        emit mouseEnter ();
    return QObject::eventFilter (obj, event);
}
