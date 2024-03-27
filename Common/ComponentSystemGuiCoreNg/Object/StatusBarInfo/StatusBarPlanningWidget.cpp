#include "StatusBarPlanningWidget.h"
#include "../MainWindow.h"

#include <QtCore/QDebug>

#define ACTIVE_COLOR            0x42aaff

StatusBarPlanningWidget::StatusBarPlanningWidget (MainWindow *mainWindow, QWidget *parent)
    : QLabel (parent)
{
    setAttribute(Qt::WA_Hover);
    _infoToolTip = nullptr;
    _alreadyShow = false;
    _mainWindow = mainWindow;

    _notActiveMute.load(":/icons/icons/planning.png");
    _activeMute = highlighPixmap(_notActiveMute);

    _notActiveNotMute.load(":/icons/icons/planning.png");
    _activeNotMute = highlighPixmap(_notActiveNotMute);

    setPixmap(_notActiveNotMute);
    setFixedSize(_activeNotMute.width() + 10, _activeNotMute.height() + 10);
    setAlignment(Qt::AlignCenter);
    setMouseTracking(true);
}

StatusBarPlanningWidget::~StatusBarPlanningWidget()
{
    _mainWindow = nullptr;

    if(_infoToolTip != nullptr) {
        _infoToolTip->close();
        delete _infoToolTip;
        _infoToolTip = nullptr;
    }
}


QPixmap StatusBarPlanningWidget::highlighPixmap (const QPixmap &defaultPixmap)
{
    //-- в прошлом getActivePixmap
    qDebug () << "[StatusBarVolume][highlighPixmap] Under construction";
    return defaultPixmap;
}

void StatusBarPlanningWidget::mousePressEvent (QMouseEvent *event)
{
    if (event->button () == Qt::LeftButton)
    {
    }
}

void StatusBarPlanningWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    if(_alreadyShow == false) {
        if(_infoToolTip == nullptr) {
            emit signalHideToolTip();

            _infoToolTip = new StatusBarPlanningWidgetToolTip(this);
            _infoToolTip->setWindowFlags(Qt::ToolTip);
            connect(_infoToolTip, SIGNAL(signalCloseToolTip()), this, SLOT(cloceToolTipWindow()));

            int posX = (_mainWindow->pos().x() + _mainWindow->width()) - _infoToolTip->width() - 30;
            int posY = (_mainWindow->pos().y() + _mainWindow->height()) - _infoToolTip->height() - 5;

            _infoToolTip->show();
            _infoToolTip->move(posX, posY);
            _infoToolTip->setWindowOpacity(0.9);
            _infoToolTip->startTimerW();

            _alreadyShow = true;
        }
    }
}

void StatusBarPlanningWidget::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    setStyleSheet("QLabel { background-color: rgba(128, 128, 128, 60); color: black; border: none; border-radius: 10px; }");
}

void StatusBarPlanningWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    _alreadyShow = false;
    QString styleSheet = "QLabel#shadowLabel {}";
    setStyleSheet(styleSheet);
}

void StatusBarPlanningWidget::cloceToolTipWindow()
{
    if(_infoToolTip != nullptr) {
        _infoToolTip->close();
        delete _infoToolTip;
        _infoToolTip = nullptr;
    }
}


void StatusBarPlanningWidget::slotHideToolTip()
{
    cloceToolTipWindow();
}
