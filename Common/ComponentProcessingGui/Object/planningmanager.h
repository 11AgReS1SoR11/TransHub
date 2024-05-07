#ifndef PLANNINGMANAGER_H
#define PLANNINGMANAGER_H

#include <QObject>
#include <QWidget>
#include <QtCore/QStack>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QReadWriteLock>
#include <QtCore/QHash>
#include "TCPProto.hpp"
#include "TCPClient.hpp"
#include <QIcon>

//!
//! \namespace Planning
//! \brief Пространство имён
//!
namespace Planning {

    struct Storage : public QObject
    {
        Q_OBJECT
    public:
        explicit Storage(QObject* parent): QObject(parent) { }

        Storage(QObject *parent, double x, double y): QObject(parent),
            NE(QPair<double, double>(x, y)) { }

        QPair<double, double> NE;
    };

    class Truck : public QObject
    {
        Q_OBJECT

    public:
        explicit Truck(QObject* parent): QObject(parent) { }

        Truck(QObject *parent, double x, double y): QObject(parent),
            NE(QPair<double, double>(x, y)) { }

        QPair<double, double> NE;
    };

    class User : public QObject
    {
        Q_OBJECT

    public:
        explicit User(QObject* parent): QObject(parent) {}

        User(QObject *parent, double x, double y): QObject(parent),
            NE(QPair<double, double>(x, y)) { }

        QPair<double, double> NE;
    };

    //!
    //! \class PlanningManager
    //! \brief Класс менеджера планирования.
    //!
    class PlanningManager : public QObject
    {
        Q_OBJECT

    public:

        //!
        //! \brief Состояние.
        //!
        enum State {
            Preparing,
            Initializing,
            Initialized,
            Running,
            Done,
            Stopping,
            Stopped
        };

        //! \brief Удаление объект
        static void deleteInstance ();

        //! \brief Вернуть инстанс.
        //! \return Инстанс.
        static PlanningManager* instance ();

        //! \brief Запуск загрузки маршрутов
        //! \brief Далее происходит часть Дани Басалаева
        void startup ();

        //! \brief Остановка загрузки маршрутов
        //! //! \brief Далее происходит часть Дани Басалаева
        void stop();

        //! \brief Инициализировать менеджер всеми объектами
        //!  которые храняться в карте на данный момент
        void init();

        //! \brief выгрузка всех маршрутов
        void shutdown ();

        //! \brief Перезапуск
        //! //! \brief Далее происходит часть Дани Басалаева
        void restart ();

        bool loadObjects ();

        //! \brief Вернуть список всех объектов
        //! \return Список всех компонентов
        static QList<QObject*> objects();

        //! \brief Вернуть флаг выполненной инициализации.
        //! \return true/false.
        bool isInitDone () const;

        //! \brief Вернуть тукущее состояние менеджера.
        State state () const;

    public:

        //! \brief Добавить объект в менеджер
        //! \param object Объект
        static bool addObject (QObject *object);

        //! \brief Удалить объект из менеджера
        //! \param object Объект
        static bool removeObject (QObject *object);

        //! \brief Удалить все объекты из менеджера
        //! \param non
        static bool removeAllObject ();

        //! \brief Вернуть список всех объектов
        //! \return Список всех объектов
        static QList<QObject*> allObjects ();

        //! \brief Вернуть список объектов по классу
        //! \return Список объектов
        template <typename T> static QList<T *> getObjects ()
        {
            QReadLocker lock (_lock);
            QList<T *> results;
            QList<QObject *> all = allObjects ();
            T *result = nullptr;
            for (QObject *obj : qAsConst (all)) {
                if ((result = /*qobject_cast*/dynamic_cast<T *> (obj)) != 0)
                    results += result;
            }
            lock.unlock();
            return results;
        }

        //! \brief Вернуть объект по классу
        //! \return Объект
        template <typename T> static T *getObject ()
        {
            QReadLocker lock (_lock);
            QList<QObject *> all = allObjects ();
            for (QObject *obj : qAsConst (all)) {
                if (T *result = /*qobject_cast*/dynamic_cast<T *> (obj))
                    return result;
            }
            lock.unlock();
            return nullptr;
        }

        //! \brief Поиск объекта с предикатом.
        //! \param predicate Предикат.
        //! \return Объект.
        template <typename T, typename Predicate> static T *getObject (Predicate predicate)
        {
            QReadLocker lock (_lock);
            QList<QObject *> all = allObjects ();
            for (QObject *obj : qAsConst (all)) {
                if (T *result = /*qobject_cast*/dynamic_cast<T *> (obj))
                    if (predicate (result))
                        return result;
            }
            _lock.unlock();
            return nullptr;
        }


    private:
        //! \brief Инстанс
        static PlanningManager *_instance;

        static QHash<QPair<double, double>, QObject*> _objects;

        static QList<QObject*> lst_objects;

        static State _state;

        //! \brief Объект блокирования на запись/чтение.
        static QReadWriteLock _lock;

        static bool _isInitDone;
        static bool _isFinishDone;

        static TCP::TCPClient client;

    protected:
        //! \brief Конструктор.
        //! \param parent Родительский объект.
        explicit PlanningManager (QObject *parent = nullptr);

        //! \brief Деструктор.
        virtual ~PlanningManager ();

        //! \brief Мьютекс
        mutable QMutex _mutex;

        QVector<QObject *> loadQueue ();

        //! \brief Объект блокирования чтения/записи списка объектов.
        //! \return Объект блокирования.
        QReadWriteLock *listLock () {
            return &_lock;
        }

    public slots:

        void slotComponentsLoaded();

        void slotTCP(TCP::Protocol::Proto proto);

    signals:
        //! \brief Сигнал о добавлении нового объекта
        //! \param obj Добавленный объект
        void aboutAddObject (QObject *obj);

        //! \brief Сигнал об удалении объекта
        //! \param obj Удаляемый объект
        void aboutToRemoveObject (QObject *obj);

        //! \brief Сигнал об окончании загрузки
        void signalComponentsLoaded();

        //! \brief Сигнал перезапуска
        void signalRestartSystem();

        //! \brief Удаление всех объектов
        void aboutToRemoveAllObjects();

        //! \brief Сигнал, о том что новый объект добавлен
        void aboutAddedObject();

        //! \brief Сигнал, о том что объект удален
        void aboutRemovedObject();

        //! \brief Сигнал о прерывании вычислений
        void breakConnection(QIcon);

        void startPlotting(const Common::Coordinates_t& CouriersCoordinates,
                           const Common::Coordinates_t& StoragesCoordinates,
                           const Common::Coordinates_t& ClientsCoordinates,
                           const Mtx::Matrix<double>& routes);

    };
}

#endif // PLANNINGMANAGER_H
