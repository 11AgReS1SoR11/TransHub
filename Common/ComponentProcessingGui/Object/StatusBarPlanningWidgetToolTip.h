#ifndef STATUSBARPLANNINGWIDGETTOOLTIP_H
#define STATUSBARPLANNINGWIDGETTOOLTIP_H

#include <QtCore/QTimer>
#include <QtWidgets/QWidget>
#include <QtGui/QMouseEvent>

namespace Ui {
    class StatusBarPlanningWidgetToolTip;
}

/*!
 * \file StatusBarPlanningWidgetToolTip.h
 * \class StatusBarPlanningWidgetToolTip
 * \brief Класс виджета всплывающей подсказки
 */
class StatusBarPlanningWidgetToolTip : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Конструктор класса
     * \param parent Виджет-родитель
     */
    explicit StatusBarPlanningWidgetToolTip(QWidget *parent = 0);

    /*!
     * \brief Деструктор класса
     */
    ~StatusBarPlanningWidgetToolTip();

private:
    Ui::StatusBarPlanningWidgetToolTip *ui; /**< объект графического интерфейса */
};


#endif // STATUSBARPLANNINGWIDGETTOOLTIP_H
