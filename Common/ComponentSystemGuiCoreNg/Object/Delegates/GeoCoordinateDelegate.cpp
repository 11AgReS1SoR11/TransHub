#include "GeoCoordinateDelegate.h"
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <cmath>


void convertToDegree (double coordinateValue_in, int &degree_out, int &minutes_out, int &seconds_out)
{
    int degree = coordinateValue_in;
    double minutes = (coordinateValue_in - degree)*60;
    if(minutes < 0)
        minutes = minutes * (-1);

    double seconds = (minutes - (int)minutes)*60;
    if(seconds < 0)
        seconds = seconds * (-1);

    degree_out = degree;
    minutes_out = (int)minutes;
    seconds_out = (int)round(seconds);
}

GeoCoordinateDelegate::GeoCoordinateDelegate (QObject *parent)
    : ISystemGuiCoreStyleItemDelegate (parent)
{}

GeoCoordinateDelegate::~GeoCoordinateDelegate ()
{}

void GeoCoordinateDelegate::recalculate (const QModelIndex &index) const
{
    QString coordinateText;
    bool isOk = false;
    double coordinate = index.data().toDouble(&isOk);
    if(isOk == true)
    {
        int degree, minutes, seconds;
        convertToDegree(coordinate, degree, minutes, seconds);

        coordinateText =
                QString ("%1° %2' %3\"")
                .arg (abs (degree), 2, 10, QChar ('0'))
                .arg (abs (minutes), 2, 10, QChar ('0'))
                .arg (abs (seconds), 2, 10, QChar ('0'));

    }
    else
        coordinateText = index.data().toString();

    QString dimension = property ("dimension").toString();
    if (dimension == "lat" || dimension == "latitude") {
        if (coordinate > 0)
            coordinateText += " N";
        else
            coordinateText += " S";
    }
    else if (dimension == "lng" || dimension == "longitude") {
        if (coordinate > 0)
            coordinateText += " E";
        else
            coordinateText += " W";
    }

    _coordinateText = coordinateText;
}

void GeoCoordinateDelegate::paint (QPainter *painter,
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

    recalculate(index);

    opt.text = _coordinateText;
    opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

    QStyle *style = opt.widget->style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

    painter->restore ();
}

QSize GeoCoordinateDelegate::sizeHint (const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    recalculate (index);

    QStyleOptionViewItem opt = option;
    initStyleOption (&opt, index);
    int newWidth = opt.fontMetrics.horizontalAdvance (_coordinateText);
    QSize newSize = QSize(newWidth + 16, opt.fontMetrics.height());
    return newSize;
}


