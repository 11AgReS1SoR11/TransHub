#include "TableColorDelegate.h"
#include <QtCore/QDebug>


TableColorDelegate::TableColorDelegate(QObject *parent)
    : ISystemGuiCoreStyleItemDelegate(parent)
{}

TableColorDelegate::~TableColorDelegate()
{}

void TableColorDelegate::paint(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption (&opt, index);
    painter->save ();

    QColor color;
    int row = index.row();

    painter->setBrush(color);
    painter->setPen(color);
    painter->drawRect(option.rect);

    opt.text = index.data().toString();
    opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    //opt.features |= QStyleOptionViewItemV2::HasDecoration;

    QStyle *style = opt.widget->style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

    painter->restore ();
}


QSize TableColorDelegate::sizeHint(const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption (&opt, index);
    int newWidth = opt.fontMetrics.horizontalAdvance (index.data().toString());
    QSize newSize = QSize(newWidth + 16, opt.fontMetrics.height());
    return newSize;
}
