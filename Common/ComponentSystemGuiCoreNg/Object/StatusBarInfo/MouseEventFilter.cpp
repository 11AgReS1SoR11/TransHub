#include "MouseEventFilter.h"
#include <QtCore/QFlags>
#include <QtCore/QDebug>

MouseEventFilter::MouseEventFilter(QObject *parent)
    : QObject(parent)
{
}

MouseEventFilter::~MouseEventFilter()
{
}

bool MouseEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        emit this->mouseMove(mouseEvent);
    }
    else if(event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        emit this->mousePress(mouseEvent);
    }
    return QObject::eventFilter(obj, event);
}
