#include "componentspec.h"
#include "componentspec_p.h"
#include "icomponent.h"
#include "icomponent_p.h"
#include "componentmanager.h"

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonArray>
#include <QtCore/QtGlobal>

using namespace ComponentsCore;
using namespace ComponentsCore::Internal;

bool ComponentDependency::operator == (const ComponentDependency &other) const
{
    return name == other.name && version == other.version && type == other.type;
}

bool ComponentSpec::provides (const QString &name, const QString &version) const {
    return  d->provides (name, version);
}

QString ComponentSpec::name () const {
    return d->_name;
}

QString ComponentSpec::version () const {
    return d->_version;
}

QString ComponentSpec::compatVersion () const {
    return d->_compatVersion;
}

QString ComponentSpec::location () const {
    return d->_location;
}

ComponentSpec::State ComponentSpec::state () const {
    return d->_state;
}

IComponent *ComponentSpec::component () const {
    return d->_component;
}

QString ComponentSpec::lastError () const
{
    if (!d->_errors.isEmpty ())
        return d->_errors.last ();
    else
        return QString ("");
}

QString ComponentSpec::lastWarning () const
{
    if (!d->_warnings.isEmpty ())
        return d->_warnings.last ();
    else
        return QString ("");
}

QStringList ComponentSpec::errors () const {
    return d->_errors;
}

void ComponentSpec::setLastError (const QString &value) {
    if (!value.trimmed ().isEmpty ())
        d->_errors.append (value);
}

QStringList ComponentSpec::warnings () const {
    return d->_warnings;
}

void ComponentSpec::setLastWarning (const QString &value) {
    if (!value.trimmed ().isEmpty ())
        d->_warnings.append (value);
}

void ComponentSpec::addArgument (const QString &arg, const QString &value)
{
    for (const auto &ad : qAsConst (d->_argumentDescriptions))
        if (ad.name == arg)
            d->_arguments.insert (arg, value);
}

void ComponentSpec::setEnabled (bool value) {
    d->_enable = value;
}

bool ComponentSpec::enabled () const {
    return d->_enable;
}

QMap<QString, QString> ComponentSpec::arguments () const {
    return d->_arguments;
}

QHash<ComponentDependency, ComponentSpec *> ComponentSpec::dependencySpecs () const {
    return d->_dependencySpecs;
}

QVector<ComponentDependency> ComponentSpec::dependencies () const {
    return d->_dependencies;
}

ComponentSpec::ComponentArgumentDescriptions ComponentSpec::argumentDescriptions () const {
    return d->_argumentDescriptions;
}

ComponentSpec::ComponentSpec ()
    : d (new ComponentSpecPrivate (this))
{}

QString ComponentSpec::fileName () const {
    return d->_fileName;
}

QString ComponentSpec::description () const {
    return d->_description;
}

bool ComponentSpec::hasError () const {
    return d->_hasError;
}

QString ComponentSpec::filePath () const {
    return d->_filePath;
}

uint ComponentsCore::qHash (const ComponentsCore::ComponentDependency &value) {
    return qHash (value.name);
}

ComponentSpecPrivate::ComponentSpecPrivate (ComponentSpec *spec)
    : q (spec)
{}

bool ComponentSpecPrivate::read (const QString &fileName)
{
    _name = _guid = _version = _location = _compatVersion
            = _filePath = _fileName = QString ();
    _state = ComponentSpec::Invalid;
    _hasError = false;

    _errors.clear ();
    _warnings.clear ();

    _component = nullptr;
    _metaData = QJsonObject ();

    QFileInfo fileInfo (fileName);
    _location = fileInfo.absolutePath ();
    _filePath = fileInfo.absoluteFilePath ();
    _fileName = fileInfo.fileName ();
    _loader.setFileName (_filePath);
    if (_loader.fileName ().isEmpty ()) {
        qWarning () << "[ComponentSpecPrivate][read] Could not open file"
                    << fileName;
        return false;
    }

    if (!readMetaData (_loader.metaData ())) {
        qCritical () << qPrintable (QString ("[ComponentSpecPrivate][read] Could not read meta data for component: %1")
                                    .arg (fileName));
        return false;
    }

    _state = ComponentSpec::Read;
    return true;
}

bool ComponentSpecPrivate::readMetaData (const QJsonObject &pluginMetaData)
{
    QJsonValue value;
    value = pluginMetaData.value (QLatin1String ("IID"));
    if (!value.isString()) {
        qWarning () << "[ComponentSpec][readMetaData] Not a component (no string IID found)";
        return false;
    }
    //if (value.toString() != PluginManager::pluginIID()) {
    //    qCDebug(pluginLog) << "Plugin ignored (IID does not match)";
    //    return false;
    //}

    value = pluginMetaData.value ("MetaData");
    if (!value.isObject ()) {
        qWarning () << "[ComponentSpec][readMetaData] Component meta data not found";
        return false;
    }
    _metaData = value.toObject ();

    //-- чтение имени
    value = _metaData.value ("Name");
    if (value.isUndefined ()) {
        qWarning () << "[ComponentSpec][readMetaData] Missing 'Name' for component";
        return false;
    }
    if (!value.isString ()) {
        qWarning () << "[ComponentSpec][readMetaData] Value 'Name' is not a string";
        return false;
    }
    _name = value.toString ();

    //-- чтение версии
    value = _metaData.value ("Version");
    if (value.isUndefined ()) {
        qWarning () << "[ComponentSpec][readMetaData] Missing 'Version' for component";
        return false;
    }
    if (!value.isString ()) {
        qWarning () << "[ComponentSpec][readMetaData] Value 'Version' is not a string";
        return false;
    }
    _version = value.toString ();

    //-- чтение версии совместимости
    value = _metaData.value ("CompatVersion");
    if (value.isUndefined ()) {
        qWarning () << "[ComponentSpec][readMetaData] Missing 'CompatVersion' for component";
        return false;
    }
    if (!value.isString ()) {
        qWarning () << "[ComponentSpec][readMetaData] Value 'CompatVersion' is not a string";
        return false;
    }
    _compatVersion = value.toString ();

    //-- чтение описания
    value = _metaData.value ("Description");
    if (value.isUndefined ()) {
        qWarning () << "[ComponentSpec][readMetaData] Missing 'Description' for component";
        return false;
    }
    if (!value.isString ()) {
        qWarning () << "[ComponentSpec][readMetaData] Value 'Description' is not a string";
        return false;
    }
    _description = value.toString ();

    //-- чтение списка зависимостей
    value = _metaData.value ("Dependencies");
    if (!value.isUndefined () && !value.isArray ()) {
        qWarning () << "[ComponentSpec][readMetaData] Value 'Dependencies' is not a object array";
        return false;
    }

    if (!value.isUndefined ())
    {
        QJsonArray array = value.toArray ();
        for (const QJsonValue &v : qAsConst (array))
        {
            if (!v.isObject ()) {
                qWarning () << "[ComponentSpec][readMetaData] Value 'Dependencies' is not a object array";
                return false;
            }

            QJsonObject dependencyObject = v.toObject ();
            ComponentDependency dep;
            value = dependencyObject.value ("Name");
            if (value.isUndefined ()) {
                qWarning () << "[ComponentSpec][readMetaData] Dependency 'Name' is missing";
                return false;
            }
            if (!value.isString ()) {
                qWarning () << "[ComponentSpec][readMetaData] Dependency 'Name' is not a string";
                return false;
            }
            dep.name = value.toString ();

            value = dependencyObject.value ("Version");
            if (value.isUndefined ()) {
                qWarning () << "[ComponentSpec][readMetaData] Dependency 'Version' is missing";
                return false;
            }
            if (!value.isString ()) {
                qWarning () << "[ComponentSpec][readMetaData] Dependency 'Version' is not a string";
                return false;
            }
            dep.version = value.toString ();
            if (!isValidVersion (dep.version)) {
                qWarning () << "[ComponentSpec][readMetaData] Invalid format version"
                            << dep.version;
                return false;
            }

            dep.type = ComponentDependency::Required;
            value = dependencyObject.value ("Type");
            if (value.isUndefined ()) {
                qWarning () << "[ComponentSpec][readMetaData] Dependency 'Type' is missing";
                return false;
            }
            if (!value.isString ()) {
                qWarning () << "[ComponentSpec][readMetaData] Dependency 'Type' is not a string";
                return false;
            }

            const QString typeValue = value.toString ();
            if (typeValue.toLower () == QLatin1String ("required")) {
                dep.type = ComponentDependency::Required;
            } else if (typeValue.toLower () == QLatin1String ("optional")) {
                dep.type = ComponentDependency::Optional;
            } else if (typeValue.toLower () == QLatin1String ("exclude")) {
                dep.type = ComponentDependency::Exclude;
            } else {
                qWarning () << "[ComponentSpec][readMetaData] Dependency 'Type' must be 'optional', 'required' or 'exclude'"
                            << typeValue;
                return false;
            }

            _dependencies.append (dep);
        }
    }

    //-- чтение списка аргументов
    value = _metaData.value ("Arguments");
    if (!value.isUndefined () && !value.isArray ()) {
        qWarning () << "[ComponentSpec][readMetaData] Value 'Arguments' is not a object array";
        return false;
    }

    if (!value.isUndefined ())
    {
        QJsonArray array = value.toArray ();
        for (const QJsonValue &v : qAsConst (array))
        {
            if (!v.isObject ()) {
                qWarning () << "[ComponentSpec][readMetaData] Value 'Arguments' is not a object array";
                return false;
            }

            QJsonObject dependencyObject = v.toObject ();

            ComponentArgumentDescription arg;
            value = dependencyObject.value ("Name");
            if (value.isUndefined ()) {
                qWarning () << "[ComponentSpec][readMetaData] Dependency 'Name' is missing";
                return false;
            }
            if (!value.isString ()) {
                qWarning () << "[ComponentSpec][readMetaData] Dependency 'Name' is not a string";
                return false;
            }
            arg.name = value.toString ();

            value = dependencyObject.value ("Parameter");
            if (!value.isUndefined () && value.isString ())
                arg.parameter = value.toString ();

            value = dependencyObject.value ("Description");
            if (value.isUndefined ()) {
                qWarning () << "[ComponentSpec][readMetaData] Dependency 'Description' is missing";
                return false;
            }
            if (!value.isString ()) {
                qWarning () << "[ComponentSpec][readMetaData] Dependency 'Description' is not a string";
                return false;
            }
            arg.description = value.toString ();

            _argumentDescriptions.append (arg);
        }
    }

    return true;
}

bool ComponentSpecPrivate::resolveDependencies (const QVector<ComponentSpec *> &specs)
{
    if (_hasError)
        return false;
    if (_state == ComponentSpec::Resolved)
        _state = ComponentSpec::Read; // Go back, so we just re-resolve the dependencies.
    if (_state != ComponentSpec::Read) {
        qCritical () << qPrintable (QString ("[ComponentsCore] Resolving dependencies for '%1' (%2) failed because state != 'Read'")
                                    .arg (_name, _version));
        _hasError = true;
        return false;
    }

    QHash<ComponentDependency, ComponentSpec *> resolved;
    for (const ComponentDependency &dep : qAsConst (_dependencies))
    {
        bool found = false;

        for (ComponentSpec *spec : qAsConst (specs))
            if (spec->provides (dep.name, dep.version)) {
                resolved.insert (dep, spec);
                found = true;
            }

        if (!found)
        {
            if (dep.type == ComponentDependency::Required)
            {
                _hasError = true;
                qCritical () << qPrintable (QString ("[ComponentsCore] Could not resolve 'required' dependency '%1' (version '%2') needed by '%3'")
                                            .arg (dep.name, dep.version, _name));
            }
            else if (dep.type == ComponentDependency::Optional)
            {
                qDebug () << qPrintable (QString ("[ComponentsCore] Could not resolve 'optional' dependency '%1' (version '%2') needed by '%3'")
                                         .arg (dep.name, dep.version, _name));
            }
            continue;
        }
        else
        {
            if (dep.type == ComponentDependency::Exclude)
            {
                _hasError = true;
                qCritical () << qPrintable (QString ("[ComponentsCore] Could not resolve 'exclude' dependency '%1' (version '%2') excluded by '%3'")
                                            .arg (dep.name, dep.version, _name));
            }
        }
    }

    if (_hasError)
        return false;

    _dependencySpecs = resolved;
    _state = ComponentSpec::Resolved;

    return true;
}

bool ComponentSpecPrivate::loadComponent ()
{
    if (_hasError)
        return false;
    //-- все зависимости найдены
    if (_state != ComponentSpec::Resolved)
    {
        if (_state == ComponentSpec::Loaded)
            return true;

        qCritical () << qPrintable (QString ("[ComponentsCore] Loading the library '%1' failed because state != 'Resolved'")
                                    .arg (_filePath));
        _hasError = true;
        return false;
    }

    //-- все зависимости загружены
    QHashIterator<ComponentDependency, ComponentSpec *> it (_dependencySpecs);
    while (it.hasNext ()) {
        it.next ();
        if (it.value ()->state () != ComponentSpec::Loaded) {
            qCritical () << qPrintable (QString ("[ComponentsCore] Loading the library '%1' failed because dependency '%2' (%3) state != 'Loaded'")
                                        .arg (_filePath, it.value ()->name (), it.value ()->version ()));
            _hasError = true;
            return false;
        }
    }

    //-- загрузка
    if (!_loader.load ()) {
        _hasError = true;
        _errors.append (_loader.errorString ());
        qCritical () << qPrintable (QString ("[ComponentsCore] Loading error: %1")
                                    .arg (_loader.errorString ()));
        return false;
    }
    auto *componentObject = qobject_cast<IComponent*> (_loader.instance ());
    if (!componentObject) {
        _hasError = true;
        qCritical () << qPrintable (QString ("[ComponentsCore] Component is not valid (does not derive from IComponent) '%1'")
                                    .arg (_filePath));
        _loader.unload ();
        return false;
    }
    _state = ComponentSpec::Loaded;
    _component = componentObject;

    _component->d->_spec = q;
    return true;
}

bool ComponentSpecPrivate::initComponent ()
{
    if (_hasError)
        return false;
    if (_state != ComponentSpec::Loaded) {
        if (_state == ComponentSpec::Initialized)
            return true;
        qCritical () << qPrintable (QString ("[ComponentsCore] Initializing the component '%1' (%2) failed because state != Loaded")
                                    .arg (_name, _version));
        _hasError = true;
        return false;
    }
    if (!_component) {
        qCritical () << qPrintable (QString ("[ComponentsCore] Internal error: have no component instance to initialize '%1' (%2)")
                                    .arg (_name, _version));
        _hasError = true;
        return false;
    }

    //-- проверка, что все зависмые компоненты уже проинициализированы
    QHashIterator<ComponentDependency, ComponentSpec *> it (_dependencySpecs);
    while (it.hasNext ()) {
        it.next ();
        if (it.value ()->state () != ComponentSpec::Initialized) {
            qCritical () << qUtf8Printable (QString ("[ComponentsCore] Loading the library '%1' failed because dependency '%2 (%3)' state != 'Loaded'")
                                            .arg (_filePath, it.value ()->name (), it.value ()->version ()));
            _hasError = true;
            return false;
        }
    }

    if (!_component->init ()) {
        _hasError = true;
        return false;
    }
    _state = ComponentSpec::Initialized;
    return true;
}

bool ComponentSpecPrivate::delayedInit ()
{
    if (_hasError)
        return false;
    if (_state != ComponentSpec::Running)
        return false;
    if (!_component) {
        qCritical () << qPrintable (QString ("[ComponentsCore] Internal error: have no %1 (%2) instance to perform delayedInit")
                                    .arg (_name, _version));
        _hasError = true;
        return false;
    }
    return _component->delayedInit ();
}

void ComponentSpecPrivate::preStartInitComponent ()
{
    if (_hasError)
        return;
    if (_state != ComponentSpec::Initialized) {
        if (_state == ComponentSpec::Running)
            return;
        qCritical () << qPrintable (QString ("[ComponentsCore] Pre start initializing the %1 (%2) failed because state != 'Initialized'")
                                    .arg (_name, _version));
        _hasError = true;
        return;
    }

    _component->preStartInit ();
    _state = ComponentSpec::Running;
}

bool ComponentSpecPrivate::provides (const QString &name, const QString &version) const
{
    if (QString::compare (name, _name, Qt::CaseInsensitive) != 0)
        return false;
    return (versionCompare (_version, version) >= 0)
            && (versionCompare (_compatVersion, version) <= 0);
}

bool ComponentSpecPrivate::enable () const {
    return _enable;
}

IComponent::ShutdownFlag ComponentSpecPrivate::stop ()
{
    if (!_component)
        return IComponent::SynchronousShutdown;
    _state = ComponentSpec::Stopped;
    return _component->aboutToShutdown ();
}

IComponent::UnloadFlag ComponentSpecPrivate::unload ()
{
    if (!_component)
        return IComponent::NonBlockingUnload;
    return _component->aboutToUnload ();
}

void ComponentSpecPrivate::kill ()
{
    if (!_component)
        return;

    delete _component;
    _component = nullptr;

    if (!_loader.unload ()) {
        qWarning () << qPrintable (QString ("[ComponentsCore] Component unload error '%1' (version %2)")
                                   .arg (_name, _version));
    } else {
        qInfo ().noquote () << qPrintable (QString ("[ComponentsCore] Component unloaded successfully '%1' (version %2)")
                                           .arg (_name, _version));
    }

    _state = ComponentSpec::Deleted;
}

int ComponentSpecPrivate::versionCompare (const QString &version1, const QString &version2)
{
    QRegExp reg1 = versionRegExp ();
    QRegExp reg2 = versionRegExp ();
    if (!reg1.exactMatch (version1)) {
        qDebug () << qPrintable (QString ("[ComponentsCore] Version is not valid (version %1)")
                                 .arg (version1));
        return 0;
    }
    if (!reg2.exactMatch (version2)) {
        qDebug () << qPrintable (QString ("[ComponentsCore] Version is not valid (version %1)")
                                 .arg (version2));
        return 0;
    }

    for (int i = 0; i < 4; ++i) {
        int number1 = reg1.cap (i + 1).toInt ();
        int number2 = reg2.cap (i + 1).toInt ();
        if (number1 < number2)
            return -1;
        if (number1 > number2)
            return 1;
    }
    return 0;
}

bool ComponentSpecPrivate::isValidVersion (const QString &version) {
    return versionRegExp ().exactMatch (version);
}

const QRegExp &ComponentSpecPrivate::versionRegExp ()
{
    static const QRegExp reg (QLatin1String ("([0-9]+)(?:[.]([0-9]+))?(?:[.]([0-9]+))?(?:_([0-9]+))?"));
    return reg;
}
