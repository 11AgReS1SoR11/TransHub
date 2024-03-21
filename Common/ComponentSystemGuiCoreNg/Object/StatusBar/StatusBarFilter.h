#ifndef STATUSBARFILTER_H
#define STATUSBARFILTER_H

#include <QtCore/QObject>
#include <QtGui/QMouseEvent>

//!
//! \file StatusBarFilter.h
//! \class StatusBarFilter
//! \brief Класс фильтра для статус бара
//! \author Ivanov Anton
//!
class StatusBarFilter : public QObject
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param parent Класс-родитель
    StatusBarFilter (QObject *parent = nullptr);
    //! \brief Деструктор класса
    virtual ~StatusBarFilter ();

protected:
    //! \brief Метод обработки событий
    //! \param obj Объект
    //! \param event Событие
    //! \return true / false
    bool eventFilter (QObject *obj, QEvent *event) override;

signals:
    //! \brief Сигнал, информирующий о получении фокуса статус баром
    void mouseEnter();
};

#endif // STATUSBARFILTER_H
