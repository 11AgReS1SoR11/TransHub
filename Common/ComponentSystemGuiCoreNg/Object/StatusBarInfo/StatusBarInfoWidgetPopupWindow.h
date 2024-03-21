#ifndef STATUSBARINFOWIDGETPOPUPWINDOW_H
#define STATUSBARINFOWIDGETPOPUPWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>

#include "SystemGuiCorePopUpElement.h"
#include "SystemWordWraper.h"

namespace Ui {
    class StatusBarInfoWidgetPopupWindow;
}

/*!
 * \file StatusBarInfoWidgetPopupWindow.h
 * \class StatusBarInfoWidgetPopupWindow
 * \brief Класс виджета всплывающего окна
 */
class StatusBarInfoWidgetPopupWindow : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief Конструктор класса
     * \param element Объект всплывающего сообщения
     * \param autoClose Автоматически скрывать
     * \param parent Виджет-родитель
     */
    explicit StatusBarInfoWidgetPopupWindow(SystemGuiCorePopUpElement* element,
                                            bool autoClose,
                                            QWidget *parent = 0);

    /*!
     * \brief Деструктор класса
     */
    ~StatusBarInfoWidgetPopupWindow();

    bool isMoved();

    /*!
     * \brief Запустить таймер авто.скрытия
     */
    void startTimerW();

    /*!
     * \brief Запустить таймер перемещения окна
     * \param finishX Конечная координата X
     * \param finishY Конечная координата Y
     */
    void startMove(int finishX, int finishY);

    /*!
     * \brief Получить координату X
     * \return Координата X
     */
    int getPosX();

    /*!
     * \brief Получить координату Y
     * \return Координата Y
     */
    int getPosY();

    //!
    //! \brief Остановить все таймеры
    //!
    void stopTimers();

protected:
    /*!
     * \brief Метод обработки нажатия кнопок мыши
     * \param event Объект события
     */
    void mousePressEvent(QMouseEvent *event);

    /*!
     * \brief Метод обработки отпускания кнопок мыши
     * \param event Объект события
     */
    void mouseReleaseEvent(QMouseEvent *event);

    /*!
     * \brief Метод обработки перемещения мыши
     * \param event Объект события
     */
    void mouseMoveEvent(QMouseEvent *event);

    /*!
     * \brief Метод обработки двойного клика на окне
     * \param event QMouseEvent
     */
    virtual void mouseDoubleClickEvent(QMouseEvent* event);

    /*!
     * \brief Метод обработки получения фокуса
     * \param event QEvent
     */
    virtual void enterEvent(QEvent *event);

    /*!
     * \brief Метод обработки потери фокуса
     * \param event QEvent
     */
    virtual void leaveEvent(QEvent *event);

private:
    Ui::StatusBarInfoWidgetPopupWindow *ui; /**< объект графического интерфейса */
    SystemGuiCorePopUpElement* _element;    /**< объект всплывающего сообщения */
    QTimer _moveTimer;                      /**< таймер, отвечающий за перемещение */
    QTimer _playTimer;                      /**< таймер, отвечающий за воспроизведение звука */
    QTimer _mouseMoveTimer;                 /**< таймер, отвечающий за перемещение мышью */

    QTimer _hideTimer;                      /**< таймер, отвечающий за закрытие окна */
    QTime  _hideTime;                       /**< время открытого окна */
    int    _elapsedTime;                    /**< время, сколько было открыто окно (мсек) */

    int _finishX;                           /**< конечная координата X */
    int _finishY;                           /**< конечная координата Y */

    bool    _dragWidget;                    /**< перетаскивание виджета */
    QPoint  _dragPos;                       /**< координаты мыши при начале таскания */

    bool    _dragSubWidget;                 /**< перетаскивание виджета (дочерние виджеты) */
    QPoint  _dragSubPos;                    /**< координаты мыши при начале таскания (дочерние виджеты) */

    bool    _isMoved;

    /*!
     * \brief Метод отрисовки окна
     */
    void paintWindow();

protected:
    /*!
     * \brief Метод отрисовки окна
     * \param event QPaintEvent
     */
    virtual void paintEvent(QPaintEvent* event);

protected slots:
    /*!
     * \brief Слот закрытия окна
     */
    void slotClose();

    /*!
     * \brief Слот перемещения окна
     */
    void slotMove();

    /*!
     * \brief Слот воспроизведения звука
     */
    void slotPlay();

    /*!
     * \brief Слот перемещения окна мышью
     */
    void slotMouseMoved();

    /*!
     * \brief Слот обработки нажатия кнопок мыши у дочерних виджетов
     * \param event Объект события
     */
    void slotMousePressEvent(QMouseEvent *event);

    /*!
     * \brief Слот обработки перемещения мыши у дочерних виджетов
     * \param event Объект события
     */
    void slotMouseMoveEvent(QMouseEvent *event);

signals:
    /*!
     * \brief Сигнал, информирующий о закрытии окна
     * \param window Всплывающее окно
     */
    void signalClose(StatusBarInfoWidgetPopupWindow* window);

    /*!
     * \brief Сигнал, информирующий о необходимости воспроизведения звука
     * \param type Тип звука
     */
    void signalPlay(ISystemGuiCorePopUpElement::ISystemGuiCorePopUpType type);

    void signalMoved();
};

#endif // STATUSBARINFOWIDGETPOPUPWINDOW_H
