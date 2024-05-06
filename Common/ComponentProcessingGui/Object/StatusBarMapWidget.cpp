#include "StatusBarMapWidget.h"
#include "StatusBarMapWidgetToolTip.h"

StatusBarMapWidget::StatusBarMapWidget(QMainWindow * mainwindow, const QPixmap & icon, QWidget * parent)
    : ISystemGuiCoreStatusBarTabWidget(mainwindow, icon, parent)
{
    _infoToolTip = new StatusBarMapWidgetToolTip(this);
    _infoToolTip->setWindowFlags(Qt::ToolTip);
}

void StatusBarMapWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    int posX = ((_mainWindow->pos().x() + _mainWindow->width()) - _infoToolTip->width() - 30 );
    int posY = (_mainWindow->pos().y() + _mainWindow->height()) - _infoToolTip->height() - 5;

    _infoToolTip->show();
    _infoToolTip->move(posX, posY);
    _infoToolTip->setWindowOpacity(0.9);
}
