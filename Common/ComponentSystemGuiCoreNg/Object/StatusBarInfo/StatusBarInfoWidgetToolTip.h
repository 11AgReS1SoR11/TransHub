#ifndef STATUSBARINFOWIDGETTOOLTIP_H
#define STATUSBARINFOWIDGETTOOLTIP_H

#include <QtCore/QTimer>
#include <QtWidgets/QWidget>
#include <QtGui/QMouseEvent>

namespace Ui {
    class StatusBarInfoWidgetToolTip;
}

/*!
 * \file StatusBarInfoWidgetToolTip.h
 * \class StatusBarInfoWidgetToolTip
 * \brief Класс виджета всплывающей подсказки
 */
class StatusBarInfoWidgetToolTip : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Конструктор класса
     * \param numActivePopUp Количество непросмотренных событий
     * \param numAllPopUp Всего событий
     * \param parent Виджет-родитель
     */
    explicit StatusBarInfoWidgetToolTip(int numActivePopUp = 0,
                                        int numAllPopUp = 0,
                                        QWidget *parent = 0);

    /*!
     * \brief Деструктор класса
     */
    ~StatusBarInfoWidgetToolTip();

    /*!
     * \brief Запустить таймер авто.скрытия
     */
    void startTimerW();

private:
    Ui::StatusBarInfoWidgetToolTip *ui; /**< объект графического интерфейса */
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

#endif // STATUSBARINFOWIDGETTOOLTIP_H
