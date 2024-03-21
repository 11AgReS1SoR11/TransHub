#include "StatusBarInfoWidgetPopupWindow.h"
#include "ui_StatusBarInfoWidgetPopupWindow.h"
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QPalette>
#include <QtGui/QPixmap>
#include <QtGui/QBrush>
#include <QtGui/QLinearGradient>
#include <QtGui/QBitmap>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtCore/QRect>
#include <QtCore/QDebug>
#include <QtCore/QTimer>

#include "MouseEventFilter.h"

#define MOVED_POP_UP            0.5 // sec
#define HIDE_AND_STOP_POP_UP    3   // sec
#define REPLAY_SOUND            30  // sec

#define ROW_HEIGHT              18

StatusBarInfoWidgetPopupWindow::StatusBarInfoWidgetPopupWindow(SystemGuiCorePopUpElement *element,
                                                               bool autoClose,
                                                               QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusBarInfoWidgetPopupWindow)
{
    ui->setupUi(this);

    _finishX = -1;
    _finishY = -1;

    _dragWidget = false;
    _dragSubWidget = false;
    _dragPos = this->pos();
    _dragSubPos = this->pos();

    _isMoved = false;

    // set filters
    MouseEventFilter *evF = new MouseEventFilter(ui->labelCaption);
    connect(evF, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(slotMouseMoveEvent(QMouseEvent*)));
    connect(evF, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(slotMousePressEvent(QMouseEvent*)));
    ui->labelCaption->installEventFilter(evF);

    evF = new MouseEventFilter(ui->labelData);
    connect(evF, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(slotMouseMoveEvent(QMouseEvent*)));
    connect(evF, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(slotMousePressEvent(QMouseEvent*)));
    ui->labelData->installEventFilter(evF);

    evF = new MouseEventFilter(ui->labelIcon);
    connect(evF, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(slotMouseMoveEvent(QMouseEvent*)));
    connect(evF, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(slotMousePressEvent(QMouseEvent*)));
    ui->labelIcon->installEventFilter(evF);
    //

    this->setWindowTitle(element->getElementCaption());
    this->setWindowIcon(element->getElementIcon_16x16());

    this->_element = element;
    connect(_element, SIGNAL(destroyed()), this, SLOT(slotClose()));

    this->ui->labelCaption->setText(this->_element->getElementCaption());
    this->ui->labelData->setText(QString("\n%1").arg(this->_element->getElementDataText()));
    this->ui->labelIcon->setPixmap(this->_element->getElementIcon_48x48().pixmap(48, 48));

    this->ui->pushButtonClose->setIcon(QIcon(":/icons/icons/icon_cancel.png"));
    this->ui->pushButtonClose->setToolTip(tr("Закрыть окно"));
    connect(ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(slotClose()));

    if(autoClose)
        ui->pushButtonClose->hide();

    int buffH = 100 + ROW_HEIGHT;
    int rowNum = 0;
    QString buffData = this->_element->getElementDataText();
    SystemWordWraper::getWrapText(buffData, 20, rowNum);
    int buffHH = ROW_HEIGHT * rowNum;

    rowNum = 0;
    buffData = this->_element->getElementCaption();
    SystemWordWraper::getWrapText(buffData, 20, rowNum);
    buffHH = buffHH + 40 + ROW_HEIGHT * rowNum;

    if(buffH < buffHH)
        buffH = buffHH;
    if(buffH > 250)
        buffH = 250;
    if(buffH < 100 + ROW_HEIGHT)
        buffH = 100 + ROW_HEIGHT;

    this->setFixedSize(310, buffH);

    connect(&_moveTimer, SIGNAL(timeout()), this, SLOT(slotMove()));
    connect(&_playTimer, SIGNAL(timeout()), this, SLOT(slotPlay()));
    connect(&_hideTimer, SIGNAL(timeout()), this, SLOT(slotClose()));

    _mouseMoveTimer.setInterval(MOVED_POP_UP * 1000); // 1 sec
    connect(&_mouseMoveTimer, SIGNAL(timeout()), this, SLOT(slotMouseMoved()));

    _playTimer.start(REPLAY_SOUND * 1000);
}

StatusBarInfoWidgetPopupWindow::~StatusBarInfoWidgetPopupWindow()
{
    delete ui;
    this->_element = nullptr;
    _mouseMoveTimer.stop();
}

bool StatusBarInfoWidgetPopupWindow::isMoved()
{
    return _isMoved;
}

void StatusBarInfoWidgetPopupWindow::startTimerW()
{
    _elapsedTime = 0;
    _hideTime.start();
    _hideTimer.start(HIDE_AND_STOP_POP_UP * 1000);
//    QTimer::singleShot(HIDE_AND_STOP_POP_UP*1000, this, SLOT(slotClose()));
}

void StatusBarInfoWidgetPopupWindow::startMove(int finishX, int finishY)
{
//    qDebug() << "Start Move X=" << finishX << "Y=" << finishY;

    _finishX = finishX;
    _finishY = finishY;
    _moveTimer.start(1);
}

int StatusBarInfoWidgetPopupWindow::getPosX()
{
    if(_finishX != -1)
        return _finishX;

    return this->pos().x();
}

int StatusBarInfoWidgetPopupWindow::getPosY()
{
    if(_finishY != -1)
        return _finishY;

    return this->pos().y();
}

void StatusBarInfoWidgetPopupWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;

    _dragWidget = true;
    _dragPos = event->globalPos();
    QWidget::mousePressEvent(event);
}

void StatusBarInfoWidgetPopupWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;

    _dragWidget = false;
    QWidget::mouseReleaseEvent(event);
}

void StatusBarInfoWidgetPopupWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(!_dragWidget)
        return;

    QPoint difPos = _dragPos - event->globalPos();
    QPoint newPos = this->pos() - difPos;
    _dragPos = event->globalPos();

    this->move(newPos);
    _isMoved = true;
    _mouseMoveTimer.start();
    QWidget::mouseMoveEvent(event);
}

void StatusBarInfoWidgetPopupWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;

    _element->activateElement();
}

void StatusBarInfoWidgetPopupWindow::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    if(!ui->pushButtonClose->isHidden())
        return;

    _elapsedTime += _hideTime.elapsed();
    _hideTimer.stop();
}

void StatusBarInfoWidgetPopupWindow::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    if(!ui->pushButtonClose->isHidden())
        return;

    if(_elapsedTime >= (HIDE_AND_STOP_POP_UP * 1000)) {
        _hideTimer.start(0);
        return;
    }

    _hideTime.start();
    _hideTimer.start(HIDE_AND_STOP_POP_UP * 1000 - _elapsedTime);
}

void StatusBarInfoWidgetPopupWindow::stopTimers()
{
    _moveTimer.stop();
    _playTimer.stop();
    _mouseMoveTimer.stop();
    _hideTimer.stop();
}

void StatusBarInfoWidgetPopupWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    this->paintWindow();
}

void StatusBarInfoWidgetPopupWindow::slotClose()
{
    _element->closeElement();
    _playTimer.stop();
    this->close();
    emit this->signalClose(this);
}

void StatusBarInfoWidgetPopupWindow::slotMove()
{
//    qDebug() << "X=" << this->pos().x() << "Y=" << this->pos().y();
//    qDebug() << "XF=" << _finishX << "YF=" << _finishY;

    if(this->pos().y() < _finishY) {
        this->move(_finishX, this->pos().y() + 1);
    } else {
        this->move(_finishX, _finishY);
        _moveTimer.stop();
        _finishX = -1;
        _finishY = -1;
    }
}

void StatusBarInfoWidgetPopupWindow::slotPlay()
{
    emit this->signalPlay(_element->getElementType());
}

void StatusBarInfoWidgetPopupWindow::slotMouseMoved()
{
    _mouseMoveTimer.stop();
    emit this->signalMoved();
}

void StatusBarInfoWidgetPopupWindow::slotMousePressEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;

    _dragSubWidget = true;
    _dragSubPos = event->globalPos();
}

void StatusBarInfoWidgetPopupWindow::slotMouseMoveEvent(QMouseEvent *event)
{
    if(!_dragSubWidget)
        return;

    QPoint difPos = _dragSubPos - event->globalPos();
    QPoint newPos = this->pos() - difPos;
    _dragSubPos = event->globalPos();

    this->move(newPos);
    _isMoved = true;
    _mouseMoveTimer.start();
}

void StatusBarInfoWidgetPopupWindow::paintWindow()
{
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
    if(painter.isActive() == false)
        return;

    QBrush painterBrush;
    QColor painterBrushColor(this->palette().brush(QPalette::Normal, QPalette::Window).color());
    painterBrush.setColor(painterBrushColor);

    painter.setBrush(this->palette().brush(QPalette::Normal, QPalette::Window));
    painter.setPen(Qt::white);
    painter.fillRect(0, 0, _widthW, _heightW, Qt::transparent);
    painter.drawPath(roundRectPath);
    painter.end();

    QPainter painterNext(this);
    if(painterNext.isActive() == false)
        return;

    painterNext.setPen(Qt::black);
    painterNext.fillRect(0, 0, _widthW, _heightW, Qt::transparent);
    painterNext.drawPixmap(0, 0, tempIcon.width(), tempIcon.height(), tempIcon);
    painterNext.setPen(QPen(Qt::white, 2));

    int buffLineY = this->ui->labelCaption->pos().y() + this->ui->labelCaption->sizeHint().height();
    painterNext.drawLine(60, buffLineY + 3, this->width() - 40, buffLineY + 3);
    painterNext.end();

    this->setMask(tempIcon.mask());
}
