#include "TableConvertDateDelegate.h"


TableConvertDateDelegate::TableConvertDateDelegate(QObject *parent)
    : ISystemGuiCoreStyleItemDelegate(parent)
{}

TableConvertDateDelegate::~TableConvertDateDelegate()
{}

void TableConvertDateDelegate::paint(QPainter *painter,
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
        timeText = QDate::fromJulianDay(dateTime).toString("dd.MM.yyyy");
    else
        timeText = index.data().toString();

    opt.text = timeText;
    opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    //opt.features |= QStyleOptionViewItemV2::HasDecoration;

    QStyle *style = opt.widget->style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

    painter->restore ();
}


QSize TableConvertDateDelegate::sizeHint(const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption (&opt, index);
    int newWidth = opt.fontMetrics.horizontalAdvance (index.data().toString());
    QSize newSize = QSize(newWidth + 16, opt.fontMetrics.height());

    return newSize;
}
