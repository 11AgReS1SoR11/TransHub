#include "BasePanelDockWidget.h"
#include <QtCore/QDebug>

#define TIMER_WAIT_MOVE         0.5 // время ожидания до показа/скрытия
#define MOVE_VALUE              100 // размер в px увеличения/уменьшения размера за 1 итерацию
#define MOUSE_AREA_ACTIVATE     20  // область в px при попадании в которую покажется панель (если атоматически скрывается)

BasePanelDockWidget::BasePanelDockWidget(const QString &title, QWidget *parent)
    : QDockWidget(title, parent),
      _mdiArea(nullptr)
{
    _state = MoveVisible;
    _isAutoHide = false;
    _isPanelVisible = true;

    _timerWaitMove.setInterval (TIMER_WAIT_MOVE * 1000);
    connect (&_timerWaitMove, SIGNAL(timeout()), this, SLOT(waitTimeOut()));
}

BasePanelDockWidget::BasePanelDockWidget (QWidget *parent)
    : QDockWidget (parent),
      _mdiArea (nullptr)
{
    _state = MoveVisible;
    _isAutoHide = false;
    _isPanelVisible = true;

    _timerWaitMove.setInterval(TIMER_WAIT_MOVE * 1000);
    connect(&_timerWaitMove, SIGNAL(timeout()), this, SLOT(waitTimeOut()));
}

BasePanelDockWidget::~BasePanelDockWidget()
{
    _mdiArea = nullptr;
}

void BasePanelDockWidget::setDockWidgetArea(Qt::DockWidgetArea area)
{
    _area = area;
}

void BasePanelDockWidget::setMdiArea(QMdiArea *mdiArea)
{
    _mdiArea = mdiArea;
}

void BasePanelDockWidget::setAutoHide(bool isAutoHide)
{
    _isAutoHide = isAutoHide;
    if(!_isAutoHide)
        _timerWaitMove.stop();
    else
        _timerWaitMove.start();
}

void BasePanelDockWidget::setPanelVisible(bool isVisible)
{
    _isPanelVisible = isVisible;
    this->setVisible(_isPanelVisible);
}

bool BasePanelDockWidget::isPanelVisible()
{
    return _isPanelVisible;
}

void BasePanelDockWidget::setPanelState(BasePanelDockWidget::MoveState state)
{
    _state = state;
}

BasePanelDockWidget::MoveState BasePanelDockWidget::panelState()
{
    return _state;
}

void BasePanelDockWidget::showEvent(QShowEvent *event)
{
    if(_isAutoHide)
        _timerWaitMove.start();

    event->accept();
}

void BasePanelDockWidget::hideEvent(QHideEvent *event)
{
    if(_timerWaitMove.isActive())
        _timerWaitMove.stop();

    event->accept();
}

void BasePanelDockWidget::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    if(!_isAutoHide)
        return;

    _timerWaitMove.stop();
}

void BasePanelDockWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    if(!_isAutoHide)
        return;

    _timerWaitMove.start();
}

int BasePanelDockWidget::currentSize()
{
    if(_area == Qt::LeftDockWidgetArea
       || _area == Qt::RightDockWidgetArea)
        return this->width();

    else if(_area == Qt::TopDockWidgetArea
            || _area == Qt::BottomDockWidgetArea)
        return this->height();

    return -1;
}

bool BasePanelDockWidget::updateShowSize(int size)
{
    bool isSuccess = false;
    if(_area == Qt::LeftDockWidgetArea
       || _area == Qt::RightDockWidgetArea) {
        int buffNewWidth = this->width() + size;
        if(buffNewWidth < _size) {
            if(!this->isVisible())
                this->setVisible(true);

            this->setFixedWidth(buffNewWidth);
        } else {
            this->setFixedWidth(_size);
            isSuccess = true;
        }

    } else if(_area == Qt::TopDockWidgetArea
              || _area == Qt::BottomDockWidgetArea) {
        int buffNewHeight = this->height() + size;
        if(buffNewHeight < _size) {
            if(!this->isVisible())
                this->setVisible(true);

            this->setFixedHeight(buffNewHeight);
        } else {
            this->setFixedHeight(_size);
            isSuccess = true;
        }
    }
    return isSuccess;
}

bool BasePanelDockWidget::updateHideSize(int size)
{
    bool isSuccess = false;
    if(_area == Qt::LeftDockWidgetArea
       || _area == Qt::RightDockWidgetArea) {
        int buffNewWidth = this->width() - size;
        if(buffNewWidth > 0) {
            this->setFixedWidth(buffNewWidth);
        } else {
            this->setFixedWidth(0);
            this->setVisible(false);
            isSuccess = true;
        }

    } else if(_area == Qt::TopDockWidgetArea
              || _area == Qt::BottomDockWidgetArea) {
        int buffNewHeight = this->height() - size;
        if(buffNewHeight > 0) {
            this->setFixedHeight(buffNewHeight);
        } else {
            this->setFixedHeight(0);
            this->setVisible(false);
            isSuccess = true;
        }
    }
    return isSuccess;
}

void BasePanelDockWidget::mousePos(const QPoint pos)
{
    if(!_isPanelVisible)
        return;

    if(_area == Qt::LeftDockWidgetArea) {
        int buffArea = MOUSE_AREA_ACTIVATE;

        if(_state != MoveInvisible)
            return;

        if(pos.x() <= buffArea) {
            this->waitTimeOut();
            return;
        }
    } else if(_area == Qt::RightDockWidgetArea) {
        int buffArea = _mdiArea.data()->width() - MOUSE_AREA_ACTIVATE;

        if(_state != MoveInvisible)
            return;

        if(pos.x() >= buffArea) {
            this->waitTimeOut();
            return;
        }
    } else if(_area == Qt::TopDockWidgetArea) {
        int buffArea = MOUSE_AREA_ACTIVATE;

        if(_state != MoveInvisible)
            return;

        if(pos.y() <= buffArea) {
            this->waitTimeOut();
            return;
        }
    } else if(_area == Qt::BottomDockWidgetArea) {
        int buffArea = _mdiArea.data()->height() - MOUSE_AREA_ACTIVATE;

        if(_state != MoveInvisible)
            return;

        if(pos.y() >= buffArea) {
            this->waitTimeOut();
            return;
        }
    }
}

void BasePanelDockWidget::showPanel()
{
    // только для нижней панели
    if(_area != Qt::BottomDockWidgetArea)
        return;
    if(!_isPanelVisible)
        return;
    if(!_isAutoHide)
        return;
    if(_state == MoveVisible)
        _timerWaitMove.stop();
    else if(_state == MoveInvisible)
        this->waitTimeOut();
}

void BasePanelDockWidget::waitTimeOut()
{
    _timerWaitMove.stop();

    if(!_isPanelVisible)
        return;

    if(_state == MoveVisible)
        _size = this->currentSize();

    while(true) {
        if(this->onMove())
            return;
    }
}

bool BasePanelDockWidget::onMove()
{
    bool isSuccess = false;
    switch(_state) {
        // скрываем...
        case MoveVisible:
        case MoveHide:
        {
            _state = MoveHide;
            isSuccess = this->updateHideSize(MOVE_VALUE);
            if(isSuccess)
                _state = MoveInvisible;

            break;
        }
        // показываем...
        case MoveInvisible:
        case MoveShow:
        {
            _state = MoveShow;
            isSuccess = this->updateShowSize(MOVE_VALUE);
            if(isSuccess)
                _state = MoveVisible;

            break;
        }
        default:
        {
            qCritical() << "[BasePanelDockWidget][onMove] Undefined panel type! Move failed!";
            isSuccess = true;
            break;
        }
    }
    return isSuccess;
}
