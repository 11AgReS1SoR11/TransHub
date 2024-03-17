//!
//! \file componentmanager.h
//!

#ifndef COMPONENTSCORE_COMPONENTMANAGER_H
#define COMPONENTSCORE_COMPONENTMANAGER_H

#include "icomponent.h"
#include "componentscore_global.h"

#include <QtCore/QStack>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QReadWriteLock>


//!
//! \namespace ComponentsCore
//! \brief Пространство имён классов менеджера компонентов
//!

//!
//! \namespace ComponentsCore::Internal
//! \brief Пространство имён внутренних классов менеджера компонентов
//!

namespace ComponentsCore {

namespace Internal {
class ComponentManagerPrivate;
}

//!
//! \class ComponentManager
//! \brief Класс менеджера компонентов.
//!
class COMPONENTSCORESHARED_EXPORT ComponentManager : public QObject
{
    Q_OBJECT

public:
    //!
    //! \brief Состояние.
    //!
    enum State {
        Invalid,
        Resolving,
        Resolved,
        Initializing,
        Running,
        Stopping,
        Stopped
    };

    //! \brief Удаление объект
    static void deleteInstance ();

    //! \brief Вернуть инстанс.
    //! \return Инстанс.
    static ComponentManager* instance ();

    //! \brief Включить профилирование.
    void enableProfiling ();

    //! \brief Запуск загрузки компонентов.
    void startup ();

    //! \brief Перезапуск системы.
    void restartSystem ();

    //! \brief Выключение системы.
    void shutdown ();

    //! \brief Вернуть список всех компонентов
    //! \return Список всех компонентов
    static QList<ComponentsCore::ComponentSpec*> components ();

    //! \brief Вернуть флаг выполненной инициализации.
    //! \return true/false.
    bool isInitDone () const;

    //! \brief Вернуть тукущее состояние менеджера.
    State state () const;

signals:
    //! \brief Сигнал об окончании загрузки
    void signalInitDone ();

    //! \brief Сигнал о загрузке компонента
    //! \param componentName Название компонента
    //! \param componentVer Версия компонента
    void signalComponentLoaded (const QString &componentName,
                                const QString &componentVer,
                                bool status,
                                const QString &error = QString ());

    //! \brief Сигнал уведомляющий о завершении загрузки (load) всех компонентов.
    void signalComponentsLoaded ();

    //! \brief Сигнал об инициализации компонента
    //! \param componentName название компонента
    //! \param componentVer Версия компонента
    //! \param status Статутс загрузки
    //! \param error Описание ошибки
    void signalComponentInitialized (const QString &componentName,
                                     const QString &componentVer,
                                     bool status,
                                     const QString &error = QString ());

    //! \brief Сигнал уведомляющий о завершении инициализации (initialize) всех компонентов.
    void signalComponentsInitialized ();

    //! \brief Сигнал о перезагрузке системы.
    void signalRestartSystem ();

    //! \brief Показать дополнительный прогресс бар.
    //! \param isShow показать / скрыть.
    void signalShowProgressBar (bool isShow);

    //! \brief Сигнал изменения максимального значения прогресс бара
    //! \param value Значение
    void signalChangeProgressBarMax (int maxValue);

    //! \brief Сигнал изменения текста прогресс бара
    //! \param text Значение
    void signalChangeProgressBarText (const QString &text);

    //! \brief Сигнал изменения значенея дополнительного прогресс бара
    //! \param value Новое значение
    void signalChangeProgressBarValue (int value);

public:
    //! \brief Добавить объект в менеджер
    //! \param object Объект
    static bool addObject (QObject *object);

    //! \brief Удалить объект из менеджера
    //! \param object Объект
    static bool removeObject (QObject *object);

    //! \brief Вернуть объект по имени
    //! \param name Имя объекта
    //! \return Объект
    static QObject *getObjectByName (const QString &name);

    //! \brief Вернуть объект по имени класса
    //! \param className имя класса объекта
    //! \return Объект
    static QObject *getObjectByClassName (const QString &className);

    //! \brief Вернуть список всех объектов
    //! \return Список всех объектов
    static QList<QObject*> allObjects ();

    //! \brief Объект блокирования чтения/записи списка объектов.
    //! \return Объект блокирования.
    static QReadWriteLock *listLock ();

    //! \brief Вернуть список объектов по классу
    //! \return Список объектов
    template <typename T> static QList<T *> getObjects ()
    {
        QReadLocker lock (listLock ());
        QList<T *> results;
        QList<QObject *> all = allObjects ();
        T *result = nullptr;
        for (QObject *obj : qAsConst (all)) {
            if ((result = /*qobject_cast*/dynamic_cast<T *> (obj)) != 0)
                results += result;
        }
        return results;
    }

    //! \brief Вернуть объект по классу
    //! \return Объект
    template <typename T> static T *getObject ()
    {
        QReadLocker lock (listLock ());
        QList<QObject *> all = allObjects ();
        for (QObject *obj : qAsConst (all)) {
            if (T *result = /*qobject_cast*/dynamic_cast<T *> (obj))
                return result;
        }
        return nullptr;
    }

    //! \brief Поиск объекта с предикатом.
    //! \param predicate Предикат.
    //! \return Объект.
    template <typename T, typename Predicate> static T *getObject (Predicate predicate)
    {
        QReadLocker lock (listLock());
        QList<QObject *> all = allObjects ();
        for (QObject *obj : qAsConst (all)) {
            if (T *result = /*qobject_cast*/dynamic_cast<T *> (obj))
                if (predicate (result))
                    return result;
        }
        return nullptr;
    }

    //! \brief Установить директории загрузки компонентов.
    //! Происходит чтение метаинформации компонентов и разрешение всех зависимостей.
    //! \param paths Директории загрузки компонентов.
    //! \param profiling Флаг включения профилирования.
    //! \return true/false.
    static bool setComponentPaths (const QStringList &paths, bool profiling = false,
                                   const QMap<QString, bool> &state = QMap<QString, bool> ());

    //! \brief Вернуть директории загрузки компонентов
    //! \return Директории загрузки компонентов
    static QStringList componentPath ();

    //! \brief Вывести в консоль информацию о всех объектах менеджера
    virtual void printAllObjects ();

    //! \brief Показать дополнительный прогресс бар
    //! \param isShow показать / скрыть
    static void showProgressBar (bool isShow);

    //! \brief Изменить максимальное значение прогресс бара
    //! \param value Значение
    static void changeProgressBarMax (int maxValue);

    //! \brief Изменить текст прогресс бара
    //! \param text Значение
    static void changeProgressBarText (const QString &text);

    //! \brief Изменить значение прогресс бара
    //! \param value Значение
    static void changeProgressBarValue (int value);

    //! \brief Добавить аргументы всем компонентам.
    //! \param arguments Список аргументов (ключ - аргумент, значение - значение аргумента).
    static void addArguments (const QMap<QString, QString> &arguments);

    //! \brief Добавить аргументы компоненту.
    //! \param component Название компонента.
    //! \param arguments Аргументы.
    static void addArguments (const QString &component, const QMap<QString, QString> &arguments);

signals:
    //! \brief Сигнал о добавлении нового объекта
    //! \param obj Добавленный объект
    void objectAdded (QObject *obj);

    //! \brief Сигнал об удалении объекта
    //! \param obj Удаляемый объект
    void aboutToRemoveObject (QObject *obj);

    //! \brief Уведомление об измененеии состояния.
    //! \param value Новое состояние.
    void stateChanged (ComponentManager::State value);

protected:
    //! \brief Конструктор.
    //! \param parent Родительский объект.
    explicit ComponentManager (QObject *parent = nullptr);

    //! \brief Деструктор.
    virtual ~ComponentManager ();

    //! \brief Мьютекс
    mutable QMutex _mutex;

private:
    //! \brief Инстанс
    static ComponentManager *_instance;
    //! \brief Текущее состояние менеджера
    State _state {State::Invalid};

    //! \brief Внутренний объект
    QScopedPointer<Internal::ComponentManagerPrivate> d;
    friend class Internal::ComponentManagerPrivate;
};

}

#endif // COMPONENTSCORE_COMPONENTMANAGER_H
