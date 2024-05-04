#ifndef STATUSBARPLANNINGWIDGET_H
#define STATUSBARPLANNINGWIDGET_H

#include "ISystemGuiCoreStatusBarTabWidget.h"

class StatusBarPlanningWidget : public ISystemGuiCoreStatusBarTabWidget
{
public:
    StatusBarPlanningWidget(QMainWindow * mainwindow, const QPixmap & icon, QWidget * parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // STATUSBARPLANNINGWIDGET_H
