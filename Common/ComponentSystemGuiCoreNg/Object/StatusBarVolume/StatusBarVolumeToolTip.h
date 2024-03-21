#ifndef STATUSBARVOLUMETOOLTIP_H
#define STATUSBARVOLUMETOOLTIP_H

#include <QtWidgets/QWidget>
#include <QtGui/QMouseEvent>
#include <QtCore/QTimer>

namespace Ui {
    class StatusBarVolumeToolTip;
}

/*!
 * \file StatusBarVolumeToolTip.h
 * \class StatusBarVolumeToolTip
 * \brief Класс виджета всплывающей подсказки
 */
class StatusBarVolumeToolTip : public QWidget
{
    Q_OBJECT
    
public:
    /*!
     * \brief Конструктор класса
     * \param isMute Есть ли звук
     * \param parent Виджет-родитель
     */
    explicit StatusBarVolumeToolTip(bool isMute, QWidget *parent = 0);

    /*!
     * \brief Деструктор класса
     */
    ~StatusBarVolumeToolTip();
    
    /*!
     * \brief Собновить состояние звука
     * \param isMute true / false
     */
    void updateState(bool isMute);

    /*!
     * \brief Запустить таймер авто.скрытия
     */
    void startTimerW();

private:
    Ui::StatusBarVolumeToolTip *ui; /**< объект графического интерфейса */
    QTimer _timerClose;             /**< таймер, отвечающий за авто.скрытие */

protected:
    /*!
     * \brief Метод обработки клика на виджете
     * \param event QMouseEvent
     */
    void mousePressEvent(QMouseEvent *event);

signals:
    /*!
     * \brief Сигнал, информирующий о закрытии всплывающей подсказки
     */
    void signalCloseToolTip();
};

#endif // STATUSBARVOLUMETOOLTIP_H
