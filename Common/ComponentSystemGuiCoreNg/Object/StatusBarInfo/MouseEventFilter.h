#ifndef MOUSEEVENTFILTER_H
#define MOUSEEVENTFILTER_H

#include <QtCore/QObject>
#include <QtGui/QMouseEvent>

/*!
 * \file MouseEventFilter.h
 * \class MouseEventFilter
 * \brief Класс фильтра для внутренних объектов
 */
class MouseEventFilter : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief Конструктор класса
     * \param parent Класс-родитель
     */
    MouseEventFilter(QObject *parent = 0);

    /*!
     * \brief Деструктор класса
     */
    virtual ~MouseEventFilter();

protected:
    /*!
     * \brief Обработка события
     * \param obj Объект
     * \param event Событие
     * \return true / false
     */
    virtual bool eventFilter(QObject *obj, QEvent *event);

signals:
    /*!
     * \brief Сигнал, информирующий о нажатии кнопки мыши
     * \param event Событие мыши
     */
    void mousePress(QMouseEvent *event);

    /*!
     * \brief Сигнал, информирующий о перемещении мыши
     * \param event Событие мыши
     */
    void mouseMove(QMouseEvent *event);
};

#endif // MOUSEEVENTFILTER_H
