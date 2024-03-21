#include "TableBoolDelegate.h"


TableBoolDelegate::TableBoolDelegate (QObject *parent)
    : ISystemGuiCoreStyleItemDelegate (parent)
{}

TableBoolDelegate::~TableBoolDelegate ()
{}

void TableBoolDelegate::paint (QPainter *painter,
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

    QString textData;
    bool _ok = false;
    int value = index.data().toInt(&_ok);
    if(_ok == true)
        this->getData(value, textData);
    else
    {
        bool valueBool = index.data().toBool();
        this->getData(valueBool, textData);
    }

    if(textData.isEmpty())
        textData = index.data().toString();

    opt.text = textData;
    opt.displayAlignment = Qt::AlignCenter | Qt::AlignVCenter;

    QStyle *style = opt.widget->style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

    painter->restore ();
}


QSize TableBoolDelegate::sizeHint (const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption (&opt, index);
    QString textData;
    int newWidth;
    bool _ok = false;
    int value = index.data().toInt(&_ok);
    if(_ok == true)
        this->getData(value, textData);

    if(textData.isEmpty())
        textData = index.data().toString();

    newWidth = opt.fontMetrics.horizontalAdvance (textData);

    QSize newSize = QSize(newWidth + 16, opt.fontMetrics.height());
    return newSize;
}

void TableBoolDelegate::getData(int value, QString &textData) const
{
    switch(value)
    {
    case 0:
    {
        textData = tr ("No");
        break;
    }
    case 1:
    {
        textData = tr ("Yes");
        break;
    }
    default:
    {
        textData = "";
        break;
    }
    }
}

void TableBoolDelegate::getData(bool value, QString &textData) const
{
    if(value == false)
        textData = tr("No");
    else
        textData = tr("Yes");
}
