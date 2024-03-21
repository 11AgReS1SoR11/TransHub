#include "HelpTextEdit.h"

#include <QtCore/QDebug>

#include <QtGui/QTextBlock>
#include <QtGui/QTextCursor>

HelpTextEdit::HelpTextEdit (QWidget *parent)
    : QTextEdit (parent)
{}

HelpTextEdit::HelpTextEdit (const QString &text, QWidget *parent)
    : QTextEdit (text, parent)
{}

HelpTextEdit::~HelpTextEdit ()
{}

void HelpTextEdit::mouseReleaseEvent (QMouseEvent *e)
{
    //qDebug() << QString("[HelpTextEdit][mouseReleaseEvent] Current text block: %1").arg(this->textCursor().block().text());

    QString anchor = anchorAt (e->pos ());
    if (!anchor.isEmpty()) {
        //qDebug() << QString("[HelpTextEdit][mouseReleaseEvent] Scroll to anchor: %1").arg(anchor);
        emit gotoAnchor (anchor);
        scrollToAnchor(anchor);
    } else {
        //qDebug() << QString("[HelpTextEdit][mouseReleaseEvent] Not founf anchor in this position");
    }

    QTextEdit::mouseReleaseEvent (e);
}
