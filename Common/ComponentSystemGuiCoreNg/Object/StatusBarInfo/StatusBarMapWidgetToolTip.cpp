#include "StatusBarMapWidgetToolTip.h"
#include "ui_StatusBarMapWidgetToolTip.h"
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QPalette>
#include <QtGui/QPixmap>
#include <QtGui/QBrush>
#include <QtGui/QLinearGradient>
#include <QtGui/QBitmap>

StatusBarMapWidgetToolTip::StatusBarMapWidgetToolTip(int numActivePopUp, int numAllPopUp, QWidget *parent) :
    ui(new Ui::StatusBarMapWidgetToolTip)
{
    ui->setupUi(this);

    QPixmap icon(":/icons/icons/novigator.png");
    this->ui->labelInfoIcon->setPixmap(icon);

    int _widthW = this->width();
    int _heightW = this->height();

    int _width = _widthW - 2;
    int _height = _heightW - 2;

    QPainterPath roundRectPath;
    roundRectPath.moveTo(_width, 5);
    roundRectPath.arcTo(_width - 10, 0, 10, 10, 0.0, 90.0);
    roundRectPath.lineTo(10, 0);
    roundRectPath.arcTo(0, 0, 10, 10, 90.0, 90.0);
    roundRectPath.lineTo(0, _height-10);
    roundRectPath.arcTo(0, _height-10, 10.0, 10.0, 180.0, 90.0);
    roundRectPath.lineTo(_width-10, _height);
    roundRectPath.arcTo(_width-10, _height-10, 10.0, 10.0, 270.0, 90.0);
    roundRectPath.closeSubpath();

    QPixmap tempIcon(_widthW, _heightW);
    tempIcon.fill(Qt::transparent);
    QPainter painter(&tempIcon);
    painter.setBrush(this->palette().brush(QPalette::Normal, QPalette::Background));
    QPen pen(Qt::white);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.fillRect(0, 0, _widthW, _heightW, Qt::transparent);
    painter.drawPath(roundRectPath);
    painter.end();

    this->render(&tempIcon,QPoint(),QRegion(), DrawChildren);
    this->setMask(tempIcon.mask());

    connect(&_timerClose, SIGNAL(timeout()), this, SIGNAL(signalCloseToolTip()));
}

StatusBarMapWidgetToolTip::~StatusBarMapWidgetToolTip()
{
    delete ui;
}

void StatusBarMapWidgetToolTip::startTimerW()
{
    _timerClose.start(3 * 1000); // 3 sec
}

void StatusBarMapWidgetToolTip::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        _timerClose.stop();
        emit this->signalCloseToolTip();
    }
}
