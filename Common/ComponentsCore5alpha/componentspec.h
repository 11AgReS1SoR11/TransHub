//!
//! \file componentspec.h
//!

#ifndef COMPONENTSCORE_COMPONENTSPEC_H
#define COMPONENTSCORE_COMPONENTSPEC_H

#include "componentscore_global.h"

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QObject>
#include <QtCore/QJsonObject>
#include <QtCore/QPluginLoader>
#include <QtCore/QVector>

namespace ComponentsCore
{
class IComponent;
namespace Internal {
class ComponentManagerPrivate;
class ComponentSpecPrivate;
}

//!
//! \struct ComponentDependency
//! \brief Структура для описания зависимостей компонента
//!
struct COMPONENTSCORESHARED_EXPORT ComponentDependency
{
    //!
    //! \brief Тип зависимости
    //!
    enum Type {
        Required,   //!< Обязательная
        Optional,   //!< Не обязательная
        Exclude     //!< Взаимоисключающий
    };

    //! \brief Конструктор
    ComponentDependency () : type (Required) {}

    QString name;       //!< Имя
    QString version;    //!< Версия
    Type type;          //!< Тип

    //! \brief Оператор сравнения
    //! \param other Сравниваемый объект
    //! \return true / false
    bool operator == (const ComponentDependency &other) const;
};

uint qHash (const ComponentsCore::ComponentDependency &value);

//!
//! \brief Описание аргумента
//!
struct COMPONENTSCORESHARED_EXPORT ComponentArgumentDescription
{
    QString name;
    QString parameter;
    QString description;
};

//!
//! \class ComponentSpec
//! \brief Класс спецификации компонента.
//!
class COMPONENTSCORESHARED_EXPORT ComponentSpec
{
public:
    //!
    //! \brief Состояния.
    //!
    enum State
    {
        Invalid,        //!< Не корретный
        Read,           //!< Считана метаинформация
        Resolved,       //!< Разрешены все зависимости
        Loaded,         //!< Загружен
        Initialized,    //!< Проинициализирован
        Running,        //!< Запущен
        Stopped,        //!< Остановлен
        Deleted         //!< Удалён
    };

    //! \brief Деструктор
    virtual ~ComponentSpec () {}

    //! \brief Вернуть название компонента
    //! \return Название компонента
    QString name () const;

    //! \brief Вернуть версию компонента
    //! \return Версия компонента
    QString version () const;

    //! \brief Вернуть версию совместимости.
    //! \return Версия совместимости.
    QString compatVersion () const;

    //! \brief Вернуть полный путь до компонента
    //! \return Полный путь до компонента
    QString location () const;

    //! \brief Вернуть путь до файла компонента
    //! \return Путь до файла компонента
    QString filePath () const;

    //! \brief Вернуть имя файла компонента
    //! \return Имя файла компонента
    QString fileName () const;

    //! \brief Вернуть описание компонента
    //! \return Описание компонента
    QString description () const;

    //! \brief Вернуть состояние
    //! \return Состояние
    State state () const;

    //! \brief Проверка совместимости версий.
    //! \param name Название компонента.
    //! \param version Версия компонента.
    //! \return true/false.
    bool provides (const QString &name, const QString &version) const;

    //! \brief Вернуть объект компонента
    //! \return Объект компонента
    IComponent *component () const;

    //! \brief Вернуть флаг наличия ошибки
    //! \return Флаг наличия ошибки
    bool hasError () const;

    //! \brief Вернуть описание последней ошибки
    //! \return Описание последней ошибки
    QString lastError () const;

    //! \brief Вернуть описание последнего предупреждения
    //! \return Описание последнего предупреждения
    QString lastWarning () const;

    //! \brief Вернуть список всех ошибок
    //! \return Список всех ошибок
    QStringList errors () const;

    //! \brief Добавить ошибку.
    //! \param value Ошибка.
    void setLastError (const QString &value);

    //! \brief Вернуть список всех предупреждений
    //! \return Список всех предупреждений
    QStringList warnings () const;

    //! \brief Добавить предупреждение.
    //! \param value Предупреждени.
    void setLastWarning (const QString &value);

    //! \brief Добавить аргумент.
    //! \param arg Аргумент.
    //! \param value Значение агумента.
    void addArgument (const QString &arg, const QString &value = QString ());

    //! \brief Включить/выключить компонент.
    //! \param value true/false
    void setEnabled (bool value);

    //! \brief Проверка включён компонент или нет.
    //! \return true/false
    bool enabled () const;

    //! \brief Вернуть список аргументов.
    //! \return Список аргументов.
    QMap<QString, QString> arguments () const;

    //! \brief Вернуть список спецификаций зависимостей компонента.
    //! \return Список спецификаций зависимостей компонента.
    QHash<ComponentDependency, ComponentSpec *> dependencySpecs () const;

    //! \brief Вернуть зависимости компонента.
    //! \return Зависимости компонента.
    QVector<ComponentDependency> dependencies () const;

    using ComponentArgumentDescriptions = QVector<ComponentArgumentDescription>;

    //! \brief Вернуть список аргументов.
    //! \return Список аргументов.
    ComponentArgumentDescriptions argumentDescriptions () const;

private:
    //! \brief Конструктор.
    ComponentSpec ();

    Internal::ComponentSpecPrivate *d {nullptr};
    friend class Internal::ComponentManagerPrivate;
    friend class Internal::ComponentSpecPrivate;
};
}

#endif // COMPONENTSCORE_COMPONENTSPEC_H
