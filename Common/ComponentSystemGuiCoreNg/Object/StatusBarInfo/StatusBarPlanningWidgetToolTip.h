#ifndef STATUSBARPLANNINGWIDGETTOOLTIP_H
#define STATUSBARPLANNINGWIDGETTOOLTIP_H


#include <QtCore/QTimer>
#include <QtWidgets/QWidget>
#include <QtGui/QMouseEvent>

namespace Ui {
    class StatusBarPlanningWidgetToolTip;
}

/*!
 * \file StatusBarInfoWidgetToolTip.h
 * \class StatusBarInfoWidgetToolTip
 * \brief Класс виджета всплывающей подсказки
 */
class StatusBarPlanningWidgetToolTip : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Конструктор класса
     * \param numActivePopUp Количество непросмотренных событий
     * \param numAllPopUp Всего событий
     * \param parent Виджет-родитель
     */
    explicit StatusBarPlanningWidgetToolTip(QWidget *parent = 0);

    /*!
     * \brief Деструктор класса
     */
    ~StatusBarPlanningWidgetToolTip();

    /*!
     * \brief Запустить таймер авто.скрытия
     */
    void startTimerW();

private:
    Ui::StatusBarPlanningWidgetToolTip *ui; /**< объект графического интерфейса */
    QTimer _timerClose;                 /**< таймер, отвечающий за авто.скрытие окна */

protected:
    /*!
     * \brief Метод обработки нажатия на окне
     * \param event QMouseEvent
     */
    void mousePressEvent(QMouseEvent *event);

signals:
    /*!
     * \brief Сигнал, информирующий о закрытии окна
     */
    void signalCloseToolTip();
};


#endif // STATUSBARPLANNINGWIDGETTOOLTIP_H
