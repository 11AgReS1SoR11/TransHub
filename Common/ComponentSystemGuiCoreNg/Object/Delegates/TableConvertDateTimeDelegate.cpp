#include "TableConvertDateTimeDelegate.h"


TableConvertDateTimeDelegate::TableConvertDateTimeDelegate(QObject *parent)
    : ISystemGuiCoreStyleItemDelegate(parent)
{}

TableConvertDateTimeDelegate::~TableConvertDateTimeDelegate()
{}

void TableConvertDateTimeDelegate::paint(QPainter *painter,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption (&opt, index);
    painter->save ();

    QColor color;
    int row = index.row();

    if(_paintBackground) {
        painter->setBrush(color);
        painter->setPen(color);
        painter->drawRect(option.rect);
    }

    bool _ok = false;
    QString timeText;
    int dateTime = index.data().toInt(&_ok);
    if(_ok == true)
        timeText = QDateTime::fromSecsSinceEpoch (dateTime).toString("dd.MM.yyyy - hh:mm:ss");
    else
        timeText = index.data().toString();

    opt.text = timeText;
    opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    //opt.features |= QStyleOptionViewItemV2::HasDecoration;

    QStyle *style = opt.widget->style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

    painter->restore ();
}


QSize TableConvertDateTimeDelegate::sizeHint(const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption (&opt, index);
    int newWidth = opt.fontMetrics.horizontalAdvance (index.data().toString());
    QSize newSize = QSize(newWidth + 16, opt.fontMetrics.height());

    return newSize;
}
