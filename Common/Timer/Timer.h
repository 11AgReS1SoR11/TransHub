#ifndef TIMEROBJECT_H
#define TIMEROBJECT_H

#include <memory>
#include <iostream>
#include <ctime>

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QString>
#include <QtCore/QUuid>

namespace Tools
{

//!
//! \brief Класс таймера для измерения времени выполнения кода.
//! \author Горбачёв Иван
//!
class NativeTimer
{
public:
    //! \brief Контруктор
    NativeTimer () { }

    //! \brief Деструктор.
    virtual ~NativeTimer () {}

    //! \brief Венуть время выполнения (в секундах)
    //! \return Время выполнения
    double mselapsed () {
        clock_gettime (CLOCK_REALTIME, &end_);
        return end_.tv_sec - beg_.tv_sec +
                (end_.tv_nsec - beg_.tv_nsec) / 1000000000.;
    }

    //! \brief Сброс таймера в 0
    void reset () { clock_gettime (CLOCK_REALTIME, &beg_); }

private:
    //! Начальная точка отсчёта
    timespec beg_;

    //! Конечгная точка отсчёта
    timespec end_;
};

static const int TimerInterval = 30;

//!
//! \brief Таймер для работы с объектом доступа к БД.
//! \author Горбачёв Иван
//!
class Timer : public QObject
{
    Q_OBJECT

public:
    enum TimerExecType {
        ExecSelect = 0,
        ExecInsert,
        ExecUpdate,
        ExecDelete,

        ExecUnknown = 0xffffffff
    };

    Timer (const QString &queryGuid, const QString &queryText, Timer::TimerExecType type)
        : _queryGuid (queryGuid)
        , _queryText (queryText)
    {
        _type = type;
        connect (&_timer, &QTimer::timeout, this, &Timer::timeOut);
    }

    virtual ~Timer ()
    {
        _timer.stop ();
        _queryGuid.clear ();
        _queryText.clear ();
    }

    void start () {
        _timer.start (TimerInterval * 1000);
    }

    void stop () {
        _timer.stop ();
    }

    //! \brief Метод собсвенно удаления!
    //! ВАЖНО! Рискованно, но по другому не удалить таймер,
    //! который создан не понятно в каком потоке!
    //! \param g Идентификатор удаляемого таймера.
    void stopAndDeleteThis (const QString &g) {
        if (g != _queryGuid)
            return;
        _timer.stop ();
        delete this;
    }

    const QString &queryGuid () const { return _queryGuid; }
    const QString &queryText () const { return _queryText; }
    Timer::TimerExecType type () { return _type; }

signals:
    void signalTimeOut ();

protected:
    QString _queryGuid;
    QString _queryText;
    TimerExecType _type;
    QTimer  _timer;

protected slots:
    void timeOut () {
        _timer.stop ();
        emit signalTimeOut ();
    }
};

}

#endif //TIMEROBJECT_H
