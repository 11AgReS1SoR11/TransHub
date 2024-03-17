#include "icomponent.h"
#include "componentmanager.h"
#include "componentmanager_p.h"
#include "componentspec.h"
#include "componentspec_p.h"

#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QMutexLocker>
#include <QtCore/QWriteLocker>
#include <QtCore/QVariant>
#include <QtCore/QUuid>
#include <QtCore/QMetaClassInfo>

#include <algorithm>

using namespace ComponentsCore;
using namespace ComponentsCore::Internal;

ComponentManager* ComponentManager::_instance = nullptr;

ComponentManager::ComponentManager (QObject* parent)
    : QObject (parent)
    , d (new ComponentManagerPrivate)
{
    d->q = this;
}

ComponentManager::~ComponentManager () {
    d->shutdown ();
}

void ComponentManager::deleteInstance ()
{
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

ComponentManager* ComponentManager::instance ()
{
    if (_instance == nullptr)
        _instance = new ComponentManager ();
    return _instance;
}

void ComponentManager::enableProfiling ()
{
    if (_instance != nullptr)
        _instance->d->initProfiling ();
}

void ComponentManager::startup () {
    instance ()->d->loadComponents ();
}

void ComponentManager::restartSystem () {
    QTimer::singleShot (5 * 1000, this, &ComponentManager::signalRestartSystem);
}

void ComponentManager::shutdown () {
    instance ()->d->shutdown ();
}

QList<ComponentsCore::ComponentSpec *> ComponentManager::components ()
{
    if (_instance == nullptr)
        return QList<ComponentsCore::ComponentSpec *> ();

    return _instance->d->_componentsSpec.toList ();
}

bool ComponentManager::isInitDone () const
{
    if (_instance == nullptr)
        return false;
    return _instance->d->_isInitDone;
}

ComponentManager::State ComponentManager::state () const
{
    return instance ()->_state;
}

bool ComponentManager::addObject (QObject *object)
{
    if (_instance != nullptr)
        return  _instance->d->addObject (object);
    return false;
}

bool ComponentManager::removeObject (QObject *object)
{
    if (_instance != nullptr)
        return  _instance->d->removeObject (object);
    return false;
}

QObject *ComponentManager::getObjectByName (const QString &name)
{
    if (_instance != nullptr)
    {
        QReadLocker locker (listLock ());
        QList<QObject *> all = allObjects ();
        for (QObject *obj : qAsConst (all)) {
            if (obj->objectName () == name)
                return obj;
        }
    }

    return nullptr;
}

QObject *ComponentManager::getObjectByClassName (const QString &className)
{
    if (_instance != nullptr)
    {
        QByteArray ba = className.toUtf8 ();
        QReadLocker locker (listLock ());
        QList<QObject *> all = allObjects ();
        for (QObject *obj : qAsConst (all)) {
            if (obj->inherits (ba.constData ()))
                return obj;
        }
    }

    return nullptr;
}

QList<QObject*> ComponentManager::allObjects ()
{
    if (_instance != nullptr)
        return _instance->d->allObjects ();

    return QList<QObject*> ();
}

QReadWriteLock *ComponentManager::listLock () {
    return  &_instance->d->_lock;
}

bool ComponentManager::setComponentPaths (const QStringList &paths, bool profiling, const QMap<QString, bool> &state)
{
    return instance ()->d->setComponentPaths (paths, profiling, state);
}

QStringList ComponentManager::componentPath ()
{
    if (_instance != nullptr)
        return _instance->d->_componentPaths;

    return QStringList ();
}

void ComponentManager::printAllObjects ()
{
    if (_instance != nullptr) {
        QReadLocker locker (listLock ());
        QList<QObject *> all = allObjects ();
        for (QObject *obj : qAsConst (all)) {
            qDebug () << obj->objectName () << obj->metaObject ()->className ();
        }
    }
}

void ComponentManager::showProgressBar (bool isShow)
{
    if (_instance != nullptr)
        emit _instance->signalShowProgressBar (isShow);
}

void ComponentManager::changeProgressBarMax (int maxValue)
{
    if (_instance != nullptr)
        emit _instance->signalChangeProgressBarMax (maxValue);
}

void ComponentManager::changeProgressBarText (const QString &text)
{
    if (_instance != nullptr)
        emit _instance->signalChangeProgressBarText (text);
}

void ComponentManager::changeProgressBarValue (int value)
{
    if (_instance != nullptr)
        emit _instance->signalChangeProgressBarValue(value);
}

void ComponentManager::addArguments (const QMap<QString, QString> &arguments)
{
    if (_instance)
        _instance->d->addArguments (arguments);
}

void ComponentManager::addArguments (const QString &component, const QMap<QString, QString> &arguments)
{
    if (_instance)
        _instance->d->addArguments (component, arguments);
}

//!=============================== ComponentManagerPrivate ===============================

ComponentManagerPrivate::ComponentManagerPrivate ()
    : QObject (nullptr)
{}

ComponentManagerPrivate::~ComponentManagerPrivate ()
{
    if (_unloadEventLoop)
        delete _unloadEventLoop;
    if (_shutdownEventLoop)
        delete _shutdownEventLoop;
}

void ComponentManagerPrivate::initProfiling ()
{
    if (_profileTimer.isNull ()) {
        _profileTimer.reset (new QElapsedTimer);
        _profileTimer->start ();
        _profileElapsedMS = 0;
        qDebug () << "[ComponentsCore] Profiling started";
    } else {
        _profilingVerbosity++;
    }
}

void ComponentManagerPrivate::profilingSummary () const
{
    if (_profileTimer.isNull ())
        return;

    QMultiMap<int, const ComponentSpec *> sorter;
    int total = 0;

    auto totalEnd = _profileTotal.constEnd ();
    for (auto it = _profileTotal.constBegin (); it != totalEnd; ++it) {
        sorter.insert (it.value (), it.key ());
        total += it.value ();
    }

    qDebug () << "[ComponentsCore] Profiling summary:";

    auto sorterEnd = sorter.constEnd ();
    for (auto it = sorter.constBegin (); it != sorterEnd; ++it)
        qDebug ("[ComponentsCore] %-22s %8dms   ( %5.2f%% )", qPrintable (it.value ()->name ()),
                it.key(), 100.0 * it.key() / total);
    qDebug ("[ComponentsCore] Total: %8dms", total);
}

void ComponentManagerPrivate::profilingReport (const char *what, const ComponentSpec *spec)
{
    if (_profileTimer.isNull ())
        return;

    const int absoluteElapsedMS = int (_profileTimer->elapsed ());
    const int elapsedMS = absoluteElapsedMS - _profileElapsedMS;
    _profileElapsedMS = absoluteElapsedMS;
    if (spec)
        qDebug ("[ComponentsCore] %-22s %-22s %8dms (%8dms)", what, qPrintable (spec->name ()), absoluteElapsedMS, elapsedMS);
    else
        qDebug ("[ComponentsCore] %-45s %8dms (%8dms)", what, absoluteElapsedMS, elapsedMS);
    if (what && *what == '<') {
        //QString tc;
        if (spec) {
            _profileTotal[spec] += elapsedMS;
            //tc = spec->name () + '_';
        }
        //tc += QString::fromUtf8 (QByteArray (what + 1));
        //TODO: gim допилить когда-нибудь
    }
}

bool ComponentManagerPrivate::addObject (QObject *object)
{
    if (!object) {
        qWarning () << "[ComponentsCore] Trying to add null object";
        return false;
    }

    QWriteLocker locker (&_lock);
    if (allObjects ().contains (object))
    {
        QString className;
        if (/*object && */object->metaObject ())
            className = object->metaObject ()->className ();
        if (!className.isEmpty ())
            qWarning () << "[ComponentsCore] Trying to add duplicate object:"
                        << className;
        else
            qWarning () << "[ComponentsCore] Trying to add duplicate object";
        return false;
    }

    _objects.append (object);
    connect (object, &QObject::destroyed,
             this, &ComponentManagerPrivate::objectDestroyed);

    if (_profilingVerbosity && !_profileTimer.isNull ()) {
        // Report a timestamp when adding an object. Useful for profiling
        // its initialization time.
        const int absoluteElapsedMS = int (_profileTimer->elapsed ());
        qDebug ("[ComponentsCore]  %-43s %8dms", object->metaObject ()->className (), absoluteElapsedMS);
    }

    locker.unlock ();

    emit q->objectAdded (object);
    return true;
}

bool ComponentManagerPrivate::removeObject (QObject* object)
{
    if (!object) {
        qWarning () << "[ComponentsCore] Trying to remove null pointer object";
        return false;
    }

    QWriteLocker locker (&_lock);
    if (!allObjects ().contains (object)) {
        qWarning () << "[ComponentsCore] Object not in the list"
                    << object->objectName ();
        return false;
    }

    disconnect (object, &QObject::destroyed,
                this, &ComponentManagerPrivate::objectDestroyed);

    emit q->aboutToRemoveObject (object);
    _objects.removeAll (object);
    return true;
}

QList<QObject*> ComponentManagerPrivate::allObjects () {
    return _objects;
}

static QStringList componentFiles (const QStringList &componentPaths)
{
    QStringList componentFiles;
    QStringList searchPaths = componentPaths;
    while (!searchPaths.isEmpty ())
    {
        const QDir dir (searchPaths.takeFirst ());
        const QFileInfoList files = dir.entryInfoList (QDir::Files);

        for (const QFileInfo &fi : files) {
            if (QLibrary::isLibrary (fi.absoluteFilePath ()))
                componentFiles.append (fi.absoluteFilePath ());
        }

        QStringList absoluteFilePaths;
        for (const QFileInfo &fi : files)
            absoluteFilePaths.append (fi.absoluteFilePath ());

        const QFileInfoList dirs = dir.entryInfoList (QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QFileInfo &fi : dirs)
            searchPaths.append (fi.absoluteFilePath ());
    }

    return componentFiles;
}

void ComponentManagerPrivate::readComponentPaths ()
{
    for (auto spec : qAsConst (_componentsSpec))
        delete spec;
    _componentsSpec.clear ();

    auto const files = componentFiles (_componentPaths);
    for (auto const &f : files)
    {
        auto spec = new ComponentSpec;
        if (!spec->d->read (f)) {
            delete spec;
            continue;
        }

        _componentsSpec.append (spec);
    }
}

void ComponentManagerPrivate::enableComponents (const QMap<QString, bool> &state)
{
    for (auto spec : qAsConst (_componentsSpec))
        if (state.contains (spec->name ()))
            spec->setEnabled (state.value (spec->name ()));
}

bool ComponentManagerPrivate::resolveDependencies ()
{
    bool found = true;

    QVector<ComponentSpec*> enabledSpec;
    for (auto spec : qAsConst (_componentsSpec))
    {
        if (spec->enabled ())
            enabledSpec.append (spec);
    }

    for (auto spec : qAsConst (enabledSpec))
    {
        if (!spec->d->resolveDependencies (enabledSpec))
            found = false;
    }

    return found;
}

void ComponentManagerPrivate::shutdown ()
{
    if (_isFinishDone)
        return;

    //-- удаление таймера, если он ещё есть
    if (_delayedInitTimer && _delayedInitTimer->isActive ()) {
        _delayedInitTimer->stop ();
        delete _delayedInitTimer;
        _delayedInitTimer = nullptr;
    }

    q->_state = ComponentManager::State::Stopping;
    emit q->stateChanged (q->_state);

    //-- остановка и запуск отложенного выключения
    for (ComponentSpec *spec : qAsConst (_initializedQueue))
    {
        profilingReport (">stop", spec);
        IComponent::ShutdownFlag flag = spec->d->stop ();
        profilingReport ("<stop", spec);

        if (IComponent::AsynchronousShutdown == flag)
        {
            _asynchronousComponents << spec;
            connect (spec->component (), &IComponent::asynchronousShutdownFinished,
                     this, &ComponentManagerPrivate::asyncShutdownFinished);

            qInfo () << qPrintable (QString ("[ComponentsCore] Component '%1' stopped 'AsynchronousShutdown'")
                                    .arg (spec->name ()));
        }
        else
        {
            qInfo () << qPrintable (QString ("[ComponentsCore] Component '%1' stopped 'SynchronousShutdown'")
                                    .arg (spec->name ()));
        }
    }

    if (!_asynchronousComponents.isEmpty ()) {
        _shutdownEventLoop = new QEventLoop;
        _shutdownEventLoop->exec ();
    }

    //-- выгрузка
    for (ComponentSpec *spec : qAsConst (_initializedQueue))
    {
        auto uflag = spec->d->unload ();
        if (uflag == IComponent::NonBlockingUnload) {
            spec->d->kill ();
        } else {
            qInfo () << qPrintable (QString ("[ComponentsCore] Component '%1' unload with wait ...")
                                    .arg (spec->name ()));

            if (!_unloadEventLoop)
                _unloadEventLoop = new QEventLoop;

            connect (spec->component (), &IComponent::unloadingPreparationFinished,
                     this, [this]() {
                if (_unloadEventLoop && _unloadEventLoop->isRunning ())
                    _unloadEventLoop->exit ();
            });

            _unloadEventLoop->exec ();
            spec->d->kill ();
        }
    }

    if (!_objects.isEmpty ())
        qInfo () << qPrintable (QString ("[ComponentsCore] There are %1 objects left in the plugin manager pool.")
                                .arg (_objects.size ()));

    _objects.clear ();
    _isFinishDone = true;

    q->_state = ComponentManager::State::Stopped;
    emit q->stateChanged (q->_state);
}

void ComponentManagerPrivate::loadComponents ()
{
    if (_isInitDone || _delayedInitTimer)
    {
        qInfo () << "[ComponentsCore] Initialization done or still in progress";
        return;
    }

    q->_state = ComponentManager::State::Initializing;
    emit q->stateChanged (q->_state);

    QVector<ComponentSpec *> const queue = loadQueue ();
    QVector<ComponentSpec *> loadedQueue;

    //-- Загрузка компонентов в порядке очереди (очередь строится на основе зависимостей).
    //-- Все успешно загруженные компоненты помещаются в очередь loadedQueue.
    for (auto spec : queue)
    {
        profilingReport (">loadComponent", spec);
        bool bload = spec->d->loadComponent ();
        profilingReport ("<loadComponent", spec);

        if (bload)
        {
            loadedQueue.append (spec);

            qInfo ().noquote () << QString ("[ComponentsCore] Component loaded successfully '%1' (version: '%2')")
                                   .arg (spec->name (), spec->version ());
            emit q->signalComponentLoaded (spec->name (), spec->version (), true);
        }
        else
        {
            qCritical () << qPrintable (QString ("[ComponentsCore] Component load failed '%1' (version: '%2')")
                                        .arg (spec->name (), spec->version ()));

            QString specErrorString;
            for (int i = 0; i < spec->errors ().size (); ++i) {
                specErrorString.append (spec->errors ().at (i));
                if (spec->errors ().size () != i + 1)
                    specErrorString.append ('\n');
            }

            emit q->signalComponentLoaded (spec->name (), spec->version (), false, specErrorString);
        }
    }

    emit q->signalComponentsLoaded ();

    //-- Инициализация компонентов в порядке очереди загрузки.
    for (ComponentSpec *spec : loadedQueue)
    {
        profilingReport (">initComponent", spec);
        bool binit = spec->d->initComponent ();
        profilingReport ("<initComponent", spec);

        if (binit)
        {
            _delayedInitQueue.prepend (spec);
            _initializedQueue.prepend (spec);

            qInfo () << qPrintable (QString ("[ComponentsCore] Component initialized successfully '%1' (version: '%2')")
                                    .arg (spec->name (), spec->version ()));
            emit q->signalComponentInitialized (spec->name (), spec->version (), true);
        }
        else
        {
            qCritical () << qPrintable (QString ("[ComponentsCore] Component '%1' (version: '%2') initialization failed")
                                        .arg (spec->name (), spec->version ()));

            profilingReport (">kill", spec);
            spec->d->kill ();
            profilingReport ("<kill", spec);

            QString specErrorString;
            for (int i = 0; i < spec->errors ().size (); ++i) {
                specErrorString.append (spec->errors ().at (i));
                if (spec->errors ().size () != i + 1)
                    specErrorString.append ('\n');
            }

            emit q->signalComponentInitialized (spec->name (), spec->version (), false, specErrorString);
        }
    }

    emit q->signalComponentsInitialized ();

    //-- запуск отложенной инициализации
    _delayedInitTimer = new QTimer;
    _delayedInitTimer->setInterval (20);
    _delayedInitTimer->setSingleShot (true);
    connect (_delayedInitTimer, &QTimer::timeout,
             this, &ComponentManagerPrivate::nextDelayedInit);
    _delayedInitTimer->start ();
}

void ComponentManagerPrivate::addArguments (const QMap<QString, QString> &arguments)
{
    QMap<QString, QString>::const_iterator it = arguments.cbegin ();
    QMap<QString, QString>::const_iterator end = arguments.cend ();

    while (it != end)
    {
        for (auto spec : qAsConst (_componentsSpec))
        {
            if (!spec->enabled ())
                continue;

            for (auto &d : spec->argumentDescriptions ())
            {
                if (d.name == it.key ())
                    spec->addArgument (it.key (), it.value ());
            }
        }
        ++it;
    }
}

void ComponentManagerPrivate::addArguments (const QString &component, const QMap<QString, QString> &arguments)
{
    for (auto spec : qAsConst (_componentsSpec))
    {
        if (!spec->enabled ())
            continue;

        if (spec->name () != component)
            continue;
        else {
            QMap<QString, QString>::const_iterator it = arguments.cbegin ();
            QMap<QString, QString>::const_iterator end = arguments.cend ();

            while (it != end)
            {
                for (auto const &d : spec->argumentDescriptions ())
                {
                    if (d.name == it.key ())
                        spec->addArgument (it.key (), it.value ());
                }
                ++it;
            }
        }
    }
}

QVector<ComponentSpec *> ComponentManagerPrivate::loadQueue ()
{
    QVector<ComponentSpec *> queue;
    for (auto spec : qAsConst (_componentsSpec))
    {
        if (!spec->enabled ())
            continue;

        QVector<ComponentSpec *> circularityCheckQueue;
        loadQueue (spec, queue, circularityCheckQueue);
    }
    return queue;
}

bool ComponentManagerPrivate::loadQueue (ComponentSpec *spec,
                                         QVector<ComponentSpec *> &queue,
                                         QVector<ComponentSpec *> &circularityCheckQueue)
{
    if (queue.contains (spec))
        return true;

    //-- check for circular dependencies
    if (circularityCheckQueue.contains (spec))
    {
        QString error = QString ("Circular dependency detected: ");
        int index = circularityCheckQueue.indexOf (spec);
        for (int i = index; i < circularityCheckQueue.size (); ++i) {
            error.append (QString (" %1 (%2) depends on")
                          .arg (circularityCheckQueue.at (i)->name (),
                                circularityCheckQueue.at (i)->version ()));
        }
        error.append (QString ("%1 (%2)").arg (spec->name (), spec->version ()));

        spec->d->_hasError = true;
        spec->d->_errors.append (error);

        qCritical () << qPrintable (QString ("[ComponentsCore] %1").arg (error));
        return false;
    }
    circularityCheckQueue.append (spec);
    //-- check if we have the dependencies
    if (spec->state () == ComponentSpec::Invalid || spec->state () == ComponentSpec::Read) {
        queue.append (spec);
        return false;
    }

    //-- add dependencies
    QHashIterator<ComponentDependency, ComponentSpec *> it (spec->dependencySpecs ());
    while (it.hasNext ())
    {
        it.next ();

        ComponentSpec *depSpec = it.value ();
        if (!loadQueue (depSpec, queue, circularityCheckQueue))
        {
            QString error = QString ("Cannot load plugin because dependency failed to load: %1 (%2) Reason: %3")
                    .arg (depSpec->name (), depSpec->version (),
                          !depSpec->errors ().isEmpty () ? depSpec->errors ().at (0) : "No information");

            spec->d->_hasError = true;
            spec->d->_errors.append (error);

            qCritical () << qPrintable (QString ("[ComponentsCore] %1").arg (error));
            return false;
        }
    }

    //-- add self
    queue.append (spec);
    return true;
}

void ComponentManagerPrivate::nextDelayedInit ()
{
    while (!_delayedInitQueue.isEmpty ())
    {
        ComponentSpec *spec = _delayedInitQueue.takeFirst ();

        profilingReport (">preStartInitComponent", spec);
        spec->d->preStartInitComponent ();
        profilingReport ("<preStartInitComponent", spec);

        profilingReport (">delayedInit", spec);
        bool delay = spec->d->delayedInit ();
        profilingReport ("<delayedInit", spec);

        if (delay)
            break;
    }

    if (_delayedInitQueue.isEmpty ())
    {
        _isInitDone = true;
        delete _delayedInitTimer;
        _delayedInitTimer = nullptr;

        profilingSummary ();

        qInfo () << "[ComponentsCore] Components core started successfully";

        q->_state = ComponentManager::State::Running;
        emit q->stateChanged (q->_state);
        emit q->signalInitDone ();
    } else {
        _delayedInitTimer->start ();
    }
}

void ComponentManagerPrivate::asyncShutdownFinished ()
{
    auto *comp = qobject_cast<IComponent *> (sender ());
    if (!comp)
        qWarning () << "[ComponentsCore] Internal error: could not find 'IComponent'";
    else {
        //qInfo ().noquote () << QString ("[ComponentsCore] Component '%1' has shutdown")
        //                       .arg (comp->spec()->name ());

        _asynchronousComponents.remove (comp->spec ());
    }

    if (_asynchronousComponents.isEmpty ())
        _shutdownEventLoop->exit ();
}

void ComponentManagerPrivate::objectDestroyed (QObject *obj)
{
    if (!obj)
        return;

    QReadLocker locker (&_lock);
    if (allObjects ().contains (obj)) {
        qWarning () << "[ComponentsCore] The object is deleted before being removed from the list";
        locker.unlock ();
        removeObject (obj);
    } else {
        locker.unlock ();
    }
}

bool ComponentManagerPrivate::setComponentPaths (const QStringList &paths, bool profiling, const QMap<QString, bool> &state)
{
    q->_state = ComponentManager::State::Resolving;
    emit q->stateChanged (q->_state);

    _componentPaths = paths;

    if (profiling)
        initProfiling ();

    //-- чтение файлов компонентов и извлечение метаинформации
    readComponentPaths ();

    if (!state.isEmpty ())
        enableComponents (state);

    //-- разрешение зависимостей
    auto const bret = resolveDependencies ();
    q->_state = ComponentManager::State::Resolved;
    emit q->stateChanged (q->_state);

    return bret;
}


