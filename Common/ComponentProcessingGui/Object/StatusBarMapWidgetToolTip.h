#ifndef STATUSBARMAPWIDGETTOOLTIP_H
#define STATUSBARMAPWIDGETTOOLTIP_H

#include <QtCore/QTimer>
#include <QtWidgets/QWidget>
#include <QtGui/QMouseEvent>

namespace Ui {
    class StatusBarMapWidgetToolTip;
}

/*!
 * \file StatusBarMapWidgetToolTip.h
 * \class StatusBarMapWidgetToolTip
 * \brief Класс виджета всплывающей подсказки
 */
class StatusBarMapWidgetToolTip : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Конструктор класса
     * \param parent Виджет-родитель
     */
    explicit StatusBarMapWidgetToolTip(QWidget *parent = 0);

    /*!
     * \brief Деструктор класса
     */
    ~StatusBarMapWidgetToolTip();

private:
    Ui::StatusBarMapWidgetToolTip *ui; /**< объект графического интерфейса */
};


#endif // STATUSBARMAPWIDGETTOOLTIP_H
