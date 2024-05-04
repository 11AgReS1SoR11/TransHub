#ifndef STATUSBARMAPWIDGET_H
#define STATUSBARMAPWIDGET_H

#include "ISystemGuiCoreStatusBarTabWidget.h"

class StatusBarMapWidget : public ISystemGuiCoreStatusBarTabWidget
{
public:
    StatusBarMapWidget(QMainWindow * mainwindow, const QPixmap & icon, QWidget * parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // STATUSBARMAPWIDGET_H
