#include "ExternalDialog.h"
#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QKeyEvent>

ExternalDialog::ExternalDialog (QWidget *childWidget, QWidget *parent)
    : QDialog (parent)
    , _childWidget (childWidget)
{
    setLayout (new QHBoxLayout);
    layout ()->setContentsMargins (0, 0, 0, 0);

    if (_childWidget)
    {
        resize (_childWidget->size ());

        _childWidget->setParent (this);
        setWindowTitle (_childWidget->windowTitle ());
        layout ()->addWidget (_childWidget);

        connect (childWidget, &QWidget::destroyed,
                this, &ExternalDialog::childDestoyed);

        //if (_debug)
        //    qDebug () << qPrintable (QString ("[ExternalDialog][ExternalDialog] Added child widget: '%1'")
        //                               .arg (_childWidget->metaObject ()->className ()));
    }
}

void ExternalDialog::childDestoyed (QObject *object)
{
    if (_childWidget == object) {
        _childWidget = nullptr;
        close ();
    }
}

void ExternalDialog::keyPressEvent (QKeyEvent *event)
{
    if (event->key () == Qt::Key_Escape)
        event->ignore ();
    else
        QDialog::keyPressEvent (event);
}

void ExternalDialog::closeEvent (QCloseEvent *event)
{
    if (!_childWidget)
        return;

    //-- метод invokeClose в списке методов child widget не найден
    int index = _childWidget->metaObject ()->indexOfMethod ("invokeClose()");
    if (index == -1) {
        event->accept ();
    } else {
        //-- в списке методов child widget найден invokeClose, значит дочерний виджет
        //-- хочет преостановить сам обработать закрытие виджета
        bool ret = false;
        if (!QMetaObject::invokeMethod (_childWidget, "invokeClose", Q_RETURN_ARG(bool,ret))) {
            event->accept ();
        } else {
            if (ret)
                event->accept ();
            else
                event->ignore ();
        }
    }
}

