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
    setFixedSize(_activeNotMute.width() + 10, _activeNotMute.height() + 10);
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

    setStyleSheet("QLabel { background-color: rgba(128, 128, 128, 60); color: black; border: none; border-radius: 10px; }");
}

void StatusBarVolume::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    _alreadyShow = false;

    if(_isMute)
        setPixmap(_notActiveMute);
    else
        setPixmap(_notActiveNotMute);


    QString styleSheet = "QLabel#shadowLabel {}";
    setStyleSheet(styleSheet);
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
