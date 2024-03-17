#ifndef COMPONENTSPECPRIVATE_H
#define COMPONENTSPECPRIVATE_H

#include "componentspec.h"
#include "icomponent.h"

#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QPluginLoader>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QVector>
#include <QtCore/QXmlStreamReader>

namespace ComponentsCore
{

class IComponent;
class ComponentManager;

namespace Internal
{

//!
//! \class ComponentSpecPrivate
//! \brief Приватный класс спецификации компонента.
//!
class COMPONENTSCORESHARED_EXPORT ComponentSpecPrivate : public QObject
{
    Q_OBJECT

public:
    //! \brief Конструктор.
    //! \param spec Спецификация.
    explicit ComponentSpecPrivate (ComponentSpec *spec);

    //! \brief Чтение метаинформации о компоненте.
    //! \param fileName Файл (полный путь).
    //! \return true/false
    bool read (const QString &fileName);

    //! \brief Чтение метаданных компонента.
    //! \param pluginMetaData Метаданные.
    //! \return true/false
    bool readMetaData (const QJsonObject &pluginMetaData);

    //! \brief Разрешение зависимостей.
    //! \param specs Список спецификаций.
    bool resolveDependencies (const QVector<ComponentSpec *> &specs);

    //! \brief Загрузка библиотеки компонента.
    //! \return true/false.
    bool loadComponent ();

    //! \brief Инициализация компонента.
    //! \return true/false.
    bool initComponent ();

    //! \brief Вернуть флаг отложенной инициализации.
    //! \return true - необходимо сделать паузу после вызова preStartInit, иначе - false.
    bool delayedInit ();

    //! \brief Предстартовая инициализация.
    void preStartInitComponent ();

    //! \brief Проверка совместимости версий.
    //! \param name Название компонента.
    //! \param version Версия компонента.
    //! \return true/false.
    bool provides (const QString &name, const QString &version) const;

    //! \brief Вернуть флаг загрузки компонента.
    //! \return true/false
    bool enable () const;

    //! \brief Вернуть флаг остановки компонента.
    IComponent::ShutdownFlag stop ();

    //! \brief Вернуть флаг выгрузки компонента.
    IComponent::UnloadFlag unload ();

    //! \brief Выгрузка компонента.
    void kill ();

    //! \brief Сравнение версий.
    //! \param version1 Версия.
    //! \param version2 Версия.
    //! \return Результат сравнения.
    //! version1 < version2 --> -1
    //! version1 > version2 --> 1
    //! version1 == version2 --> 0
    //! не корректные версии --> 0
    static int versionCompare (const QString &version1, const QString &version2);

    //! \brief Проверка корректности версии.
    //! \param version Версия.
    //! \return true/false
    static bool isValidVersion (const QString &version);

    QString _name;          //!< Имя компонента.
    QString _guid;          //!< Идентификатор компонента.
    QString _version;       //!< Версия компонента.
    QString _compatVersion; //!< Версия совместимости.
    QString _location;      //!< Полный путь до компонента
    QString _filePath;      //!< Путь до файла компонента
    QString _fileName;      //!< Имя файла компонента
    QString _description;   //!< Описание.
    ComponentSpec::State _state {ComponentSpec::Invalid};   //!< Состояние
    bool _hasError {false}; //!< Флаг наличия ошибки
    QStringList _errors;    //!< Список ошибок
    QStringList _warnings;  //!< Список предупреждений
    QJsonObject _metaData;  //!< Метаинформация.
    QPluginLoader _loader;  //!< Загрузчик компонента.

    //! Аргументы.
    QMap<QString, QString> _arguments;

    //! \brief Список зависмостей.
    QVector<ComponentDependency> _dependencies;

    //! \brief Спецификации зависимостей.
    QHash<ComponentDependency, ComponentSpec*> _dependencySpecs;

    //! \brief Список агрументов.
    ComponentSpec::ComponentArgumentDescriptions _argumentDescriptions;

    //! \brief Объект компонента
    IComponent* _component {nullptr};

    //! Флаг загрузки компонента.
    bool _enable {true};

private:
    ComponentSpec *q {nullptr};
    //! \brief Регулярное выражения описания версии.
    static const QRegExp &versionRegExp ();
};

}

}

#endif // COMPONENTSPECPRIVATE_H
