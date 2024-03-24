#ifndef STATUSBARMAPWIDGETTOOLTIP_H
#define STATUSBARMAPWIDGETTOOLTIP_H

#include <QtCore/QTimer>
#include <QtWidgets/QWidget>
#include <QtGui/QMouseEvent>

namespace Ui {
    class StatusBarMapWidgetToolTip;
}

/*!
 * \file StatusBarInfoWidgetToolTip.h
 * \class StatusBarInfoWidgetToolTip
 * \brief Класс виджета всплывающей подсказки
 */
class StatusBarMapWidgetToolTip : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Конструктор класса
     * \param numActivePopUp Количество непросмотренных событий
     * \param numAllPopUp Всего событий
     * \param parent Виджет-родитель
     */
    explicit StatusBarMapWidgetToolTip(int numActivePopUp = 0,
                                        int numAllPopUp = 0,
                                        QWidget *parent = 0);

    /*!
     * \brief Деструктор класса
     */
    ~StatusBarMapWidgetToolTip();

    /*!
     * \brief Запустить таймер авто.скрытия
     */
    void startTimerW();

private:
    Ui::StatusBarMapWidgetToolTip *ui; /**< объект графического интерфейса */
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


#endif // STATUSBARMAPWIDGETTOOLTIP_H
