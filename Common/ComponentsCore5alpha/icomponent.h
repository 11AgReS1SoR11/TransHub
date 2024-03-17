//!
//! \file icomponent.h
//!

#ifndef COMPONENTSCORE_ICOMPONENT_H
#define COMPONENTSCORE_ICOMPONENT_H

#include "componentspec.h"
#include <QtCore/QtPlugin>
#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QStringList>

namespace ComponentsCore {

namespace Internal {
    class IComponentPrivate;
    class ComponentSpecPrivate;
}

class ComponentManager;

//!
//! \class IComponent
//! \brief Базовый класс для представления системного компонента.
//!
class COMPONENTSCORESHARED_EXPORT IComponent : public QObject
{
    Q_OBJECT

public:
    enum ShutdownFlag {
        SynchronousShutdown,
        AsynchronousShutdown
    };

    enum UnloadFlag {
        NonBlockingUnload,
        BlockingUnload
    };

    //! \brief Конструктор
    IComponent ();

    //! \brief Деструктор
    virtual ~IComponent ();

    //! \brief Инициализация компонента.
    //! \return true / false
    virtual bool init () = 0;

    //! \brief Вернуть флаг остановка компонента.
    //! \return Флаг остановки.
    virtual ShutdownFlag aboutToShutdown () { return SynchronousShutdown; }

    //! \brief Вернуть флаг выгрузки компоненета.
    //! \return Флаг выгрузки компоненета.
    virtual UnloadFlag aboutToUnload () { return NonBlockingUnload; }

    //! \brief Необходима ли пауза после вызова preStartInit.
    //! \return true/false
    virtual bool delayedInit () { return false; }

    //! \brief Предстартовая инициализация
    //! Выполняется через 20 мс после того, как все зависимости проинициализированы.
    virtual void preStartInit () {}

    //! \brief Установить последнюю ошибку компонента
    //! \param error Последняя ошибка компонента
    void setLastError (const QString &error);

    //! \brief Вернуть последнюю ошибку компонента
    //! \return Последняя ошибка компонента
    QString lastError () const;

    //! \brief Вернуть список ошибок компонента
    //! \return Список ошибок компонента
    QStringList errors () const;

    //! \brief Установить последнее предупреждение компонента
    //! \param warning Последнее предупреждение компонента
    void setLastWarning (const QString &warning);

    //! \brief Вернуть последнее предупреждение компонента
    //! \return Последнее предупреждение компонента
    QString lastWarning () const;

    //! \brief Вернуть список предупреждений компонента
    //! \return Список предупреждений компонента
    QStringList warnings () const;

    //! \brief Вернуть спецификацию компонента.
    //! \return Спецификация компонента.
    ComponentSpec *spec () const;

    //! \brief Вернуть список аргументов.
    //! \return Список аргументов.
    QMap<QString, QString> arguments () const;

public:
    //! \brief Добавление объекта в список экспортируемых объектов
    //! \param object Объект
    virtual void addObject (QObject *object);

    //! \brief Удаление объекта из списка экспортируемых объектов.
    //! \param object
    virtual void removeObject (QObject *object);

signals:
    //! \brief Сигнал о завершении асинхронного завершения работы компонента.
    void asynchronousShutdownFinished ();

    //! \brief Сигнал о завершении подготовки в выгрузке компонента.
    void unloadingPreparationFinished ();

protected slots:
    //! \brief Задать отображение дополнительного прогресс бара при загрузке компонента
    //! \param show true / false
    void showProgressBar (bool isShow);

    //! \brief Задать максимальное значение прогресс бара
    //! \param maxValue максимальное значение прогресс бара
    void setProgressBarMax (int maxValue);

    //! \brief Задать текст прогресс бара
    //! \param text Текст прогресс бара
    void setProgressBarText (const QString &text);

    //! \brief Изменить значение прогресс бара
    //! \param value Значение
    void changeProgressBarValue (int value);

private:
    Internal::IComponentPrivate *d;
    friend class Internal::ComponentSpecPrivate;
};

}

// The macros Q_EXPORT_PLUGIN, Q_EXPORT_PLUGIN2 become obsolete in Qt 5.
#if QT_VERSION >= 0x050000
#    if defined(Q_EXPORT_PLUGIN)
#        undef Q_EXPORT_PLUGIN
#        undef Q_EXPORT_PLUGIN2
#    endif
#    define Q_EXPORT_PLUGIN(plugin)
#    define Q_EXPORT_PLUGIN2(function, plugin)
#else
#    define Q_PLUGIN_METADATA(x)
#endif

#endif // COMPONENTSCORE_ICOMPONENT_H
