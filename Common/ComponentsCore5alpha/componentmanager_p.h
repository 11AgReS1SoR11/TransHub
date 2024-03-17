//!
//! \file componentmanager_p.h
//!

#ifndef COMPONENTSCORE_COMPONENTMANAGER_PRIVATE_H
#define COMPONENTSCORE_COMPONENTMANAGER_PRIVATE_H

#include <QtCore/QTimer>
#include <QtCore/QHash>
#include <QtCore/QStack>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QEventLoop>
#include <QtCore/QSet>
#include <QtCore/QReadWriteLock>
#include <QtCore/QScopedPointer>
#include <QtCore/QElapsedTimer>

#include "icomponent.h"

namespace ComponentsCore {

class ComponentManager;

namespace Internal {

//!
//! \class ComponentManagerPrivate
//! \brief Внутренний класс менеджера компонентов.
//!
class ComponentManagerPrivate : public QObject
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    ComponentManagerPrivate ();

    //! \brief Деструктор.
    virtual ~ComponentManagerPrivate ();

    //! \brief Инициализация профилирование.
    void initProfiling ();

    //! \brief Вывод в терминал результатов профилирования.
    void profilingSummary () const;

    //! \brief profilingReport
    //! \param what
    //! \param spec
    void profilingReport (const char *what, const ComponentSpec *spec = nullptr);

    //! \brief Добавить объект в менеджер
    //! \param object Объект
    //! \return true/false
    bool addObject (QObject *object);

    //! \brief Удалить объект из менеджера
    //! \param object Объект
    //! \return true/false
    bool removeObject (QObject *object);

    //! \brief Вывести в консоль информацию о всех объектах менеджера
    void printAllObjects ();

    //! \brief Установить директории загрузки компонентов.
    //! Происходит чтение метаинформации компонентов и разрешение всех зависимостей.
    //! \param paths Директории загрузки компонентов.
    //! \param profiling Флаг включения профилирования.
    //! \return true/false.
    bool setComponentPaths (const QStringList &paths, bool profiling, const QMap<QString, bool> &state);

    //! \brief Вернуть список всех объектов
    //! \return Список всех объектов
    QList<QObject*> allObjects ();

    //! \brief Чтение путей с компонентами.
    //! Чтение метаинформации о компоненте.
    //! ВАЖНО! На данном этапе не решается задача проверки
    //! зависмостей или инициализация.
    void readComponentPaths ();

    //! \brief Включить/выключить компонента.
    //! \param state Перечень названий компонентов с названиями и состоянием.
    void enableComponents (const QMap<QString, bool> &state);

    //! \brief Разрешение зависимостей.
    //!
    //! Разрешение зависимостей по всем считаннмы компонентам.
    //! Важно понимать, что компоненты ещё не загружены, а есть только
    //! метаинформация о них в списке \see _componentsSpec. На основе
    //! этой метаинформации и проверяются все зависимости.
    bool resolveDependencies ();

    //! \brief Остановка
    void shutdown ();

    //! \brief Загрузка компонентов.
    //!
    //! Загрузка - это ...
    //! 1. Построение очереди загрузки на основе \see _componentsSpec;
    //! 2. Выполнение непосредственно загрузки компонента (QPluginLoader::load);
    //! 3. Выполнение инифциализации компонента;
    //! 4. Выполнение отложенной инициализации.
    //!
    //! \return true / false
    void loadComponents ();

    //! \brief Добавить аргументы всем компонентам.
    //! \param arguments Список аргументов (ключ - аргумент, значение - значение аргумента).
    void addArguments (const QMap<QString, QString> &arguments);

    //! \brief Добавить аргументы компоненту.
    //! \param arguments Список аргументов (ключ - аргумент, значение - значение аргумента).
    void addArguments (const QString &component, const QMap<QString, QString> &arguments);

    //! \brief Формирование очереди загрузки.
    QVector<ComponentSpec *> loadQueue ();

    //! \brief Формирование очереди загрузки.
    bool loadQueue (ComponentSpec *spec,
                    QVector<ComponentSpec *> &queue,
                    QVector<ComponentSpec *> &circularityCheckQueue);

    //! Таймер для отложенной инициализации.
    QTimer *_delayedInitTimer {nullptr};

    //! Очередь отложенной инициализации.
    QVector<ComponentSpec *> _delayedInitQueue;

    //! Очередь компонентов, которые прошли инициализацию.
    QVector<ComponentSpec *> _initializedQueue;

    //! \brief Все объекты, экспортируемые компонентами
    QList<QObject*> _objects;

    //! \brief Список директорий, где искать компоненты.
    QStringList _componentPaths;

    //! \brief Список спецификаций компонентов
    QVector<ComponentSpec*> _componentsSpec;

    //! \brief Список компонентов с асинхронным выключением.
    QSet<ComponentSpec *> _asynchronousComponents;

    //! \brief Цикл обработки событий во время остановки компонента.
    QEventLoop *_shutdownEventLoop {nullptr};

    //! \brief Цикл обработки событий во время выгрузки компонента.
    QEventLoop *_unloadEventLoop {nullptr};

    //! Флаг окончания инициализации.
    bool _isInitDone {false};

    //! Флаг окончания работы всех компонентов.
    bool _isFinishDone {false};

    //! \brief Объект блокирования на запись/чтение.
    mutable QReadWriteLock _lock;

    //! \brief Основной объект
    ComponentManager *q {nullptr};

    //!
    QScopedPointer<QElapsedTimer> _profileTimer;

    //!
    QHash<const ComponentSpec *, int> _profileTotal;

    //!
    int _profileElapsedMS {0};

    //!
    unsigned _profilingVerbosity {0};

public slots:
    //! \brief Обработка отложенной инициализации.
    void nextDelayedInit ();

    //! \brief Обработчик асинзронного завершения работы компонента.
    void asyncShutdownFinished ();

    //! \brief Обработчик удаления объекта из списка опубликованных объектов.
    //! \param obj Удалённый объект.
    void objectDestroyed (QObject *obj = nullptr);
};

}
}

#endif // COMPONENTSCORE_COMPONENTMANAGER_PRIVATE_H
