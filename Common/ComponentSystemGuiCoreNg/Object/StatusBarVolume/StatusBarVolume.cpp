#include "StatusBarVolume.h"
#include "../MainWindow.h"

#include <QtCore/QDebug>

#define ACTIVE_COLOR            0x42aaff

StatusBarVolume::StatusBarVolume (MainWindow *mainWindow, QWidget *parent)
    : QLabel (parent)
{
    _infoToolTip = nullptr;
    _alreadyShow = false;
    _isMute = false;
    _mainWindow = mainWindow;

    _notActiveMute.load(":/icons/icons/Sound_Mute.png");
    _activeMute = highlighPixmap(_notActiveMute);

    _notActiveNotMute.load(":/icons/icons/Sound_Up.png");
    _activeNotMute = highlighPixmap(_notActiveNotMute);

    setPixmap(_notActiveNotMute);
    setFixedSize(_activeNotMute.width(), _activeNotMute.height());
    setAlignment(Qt::AlignCenter);
    setMouseTracking(true);
}

StatusBarVolume::~StatusBarVolume()
{
    _mainWindow = nullptr;

    if(_infoToolTip != nullptr) {
        _infoToolTip->close();
        delete _infoToolTip;
        _infoToolTip = nullptr;
    }
}

bool StatusBarVolume::isMute()
{
    return _isMute;
}

void StatusBarVolume::setMute(bool isMute)
{
    _isMute = isMute;
    if(_isMute)
        setPixmap(_notActiveMute);
    else
        setPixmap(_notActiveNotMute);

    emit signalVolumeChanged(_isMute);
}

QPixmap StatusBarVolume::highlighPixmap (const QPixmap &defaultPixmap)
{
    //-- в прошлом getActivePixmap
    qDebug () << "[StatusBarVolume][highlighPixmap] Under construction";
    return defaultPixmap;

//    int _widthW = defaultPixmap.width() + 2;
//    int _heightW = defaultPixmap.height() + 2;

//    QPixmap m_openedPix = defaultPixmap.scaled(_widthW, _heightW);
//    QPixmap alpha = m_openedPix.alphaChannel();
//    QImage img = m_openedPix.toImage();

//    QColor activeColor;
//    activeColor.setRgb(ACTIVE_COLOR);

//    for(int w = 0; w < img.rect().right() + 1; w++)
//    {
//        for(int h = 0; h < img.rect().bottom() + 1; h++)
//        {
//            QColor col(img.pixel(w,h));
//            col.setHsv(col.hue(), 0, col.value(), col.alpha());
//            img.setPixel(w,h, activeColor.rgb());
//        }
//    }
//    m_openedPix = QPixmap::fromImage(img);
//    m_openedPix.setAlphaChannel(alpha);

//    // формируем итоговую картинку
//    QPixmap tempIcon(_widthW, _heightW);
//    tempIcon.fill(Qt::transparent);
//    QPainter painter(&tempIcon);
//    painter.fillRect(0, 0, _widthW, _heightW, Qt::transparent);
//    painter.drawPixmap(0, 0, _widthW, _heightW, m_openedPix);
//    painter.drawPixmap(1, 1, _widthW - 2, _heightW - 2, defaultPixmap);
//    painter.end();

//    return tempIcon;
}

void StatusBarVolume::mousePressEvent (QMouseEvent *event)
{
    if (event->button () == Qt::LeftButton)
    {
        _isMute = !_isMute;

        if(_infoToolTip)
            cloceToolTipWindow();

        if(_isMute)
            setPixmap(_activeMute);
        else
            setPixmap(_activeNotMute);

        emit signalVolumeChanged(_isMute);
    }
}

void StatusBarVolume::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    if(_isMute)
        setPixmap(_activeMute);
    else
        setPixmap(_activeNotMute);

    if(_alreadyShow == false) {
        if(_infoToolTip == nullptr) {
            emit signalHideToolTip();

            _infoToolTip = new StatusBarVolumeToolTip(_isMute, this);
            _infoToolTip->setWindowFlags(Qt::ToolTip);
            connect(_infoToolTip, SIGNAL(signalCloseToolTip()), this, SLOT(cloceToolTipWindow()));

            int posX = (_mainWindow->pos().x() + _mainWindow->width()) - _infoToolTip->width() - 30;
            int posY = (_mainWindow->pos().y() + _mainWindow->height()) - _infoToolTip->height();

            _infoToolTip->show();
            _infoToolTip->move(posX, posY);
            _infoToolTip->setWindowOpacity(0.9);
            _infoToolTip->startTimerW();

            _alreadyShow = true;
        }
    }
}

void StatusBarVolume::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
}

void StatusBarVolume::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    _alreadyShow = false;

    if(_isMute)
        setPixmap(_notActiveMute);
    else
        setPixmap(_notActiveNotMute);
}

void StatusBarVolume::cloceToolTipWindow()
{
    if(_infoToolTip != nullptr) {
        _infoToolTip->close();
        delete _infoToolTip;
        _infoToolTip = nullptr;
    }
}


void StatusBarVolume::slotHideToolTip()
{
    cloceToolTipWindow();
}
