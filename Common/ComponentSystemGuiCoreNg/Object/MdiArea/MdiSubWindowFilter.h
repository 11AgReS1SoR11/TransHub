#ifndef MDISUBWINDOWFILTER_H
#define MDISUBWINDOWFILTER_H

#include <QtCore/QObject>

#include <QtWidgets/QMdiSubWindow>
#include <QtGui/QMouseEvent>

//!
//! \file MdiSubWindowFilter.h
//! \class MdiSubWindowFilter
//! \brief Класс фильтра для внутренних окон
//!
class MdiSubWindowFilter : public QObject
{
    Q_OBJECT

public:
    //! \brief Конструктор класса
    //! \param subWindow Внутренее окно
    //! \param parent Класс-родитель
    MdiSubWindowFilter (QMdiSubWindow *subWindow, QObject *parent = nullptr);

    //! \brief Деструктор класса
    virtual ~MdiSubWindowFilter ();

signals:
    //! \brief Сигнал, информирующий о позиции мыши
    //! \param pos Позиция мыши
    void mousePos (const QPoint pos);

public slots:
    void OnDeleteMdiSubWindow();

protected:
    QMdiSubWindow *_subWindow;  //!< внутренее окно

    //! \brief Обработка события
    //! \param obj Объект
    //! \param event Событие
    //! \return true / false
    bool eventFilter (QObject *obj, QEvent *event) override;
};

#endif // MDISUBWINDOWFILTER_H


