#include "planningmanager.h"

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
#include <iostream>

using namespace Planning;

PlanningManager* PlanningManager::_instance = nullptr;

QHash<QPair<double, double>, QObject*> PlanningManager:: _objects = QHash<QPair<double, double>, QObject*>();

QReadWriteLock PlanningManager:: _lock(QReadWriteLock::RecursionMode::NonRecursive);

bool PlanningManager:: _isFinishDone = false;

bool PlanningManager:: _isInitDone = false;

PlanningManager::State PlanningManager:: _state = PlanningManager::State::Preparing;

PlanningManager::PlanningManager (QObject* parent)
    : QObject (parent)
{
    connect (this, &PlanningManager::aboutToRemoveObject, this, [this](QObject* object) {
        removeObject(object);
    });
    connect (this, &PlanningManager::aboutAddObject, this, [this](QObject* object) {
        addObject(object);
    });

    connect (this, &PlanningManager::signalComponentsLoaded, this, [this]() {
        slotComponentsLoaded();
    });

    connect (this, &PlanningManager::aboutToRemoveAllObjects, this, [this]() {
        removeAllObject();
    });
}

PlanningManager::~PlanningManager () {}

void PlanningManager::deleteInstance ()
{
    if (_instance) {

        QHashIterator<QPair<double, double>, QObject*> iter(_objects);
        while(iter.hasNext()) {
            iter.next();
            delete iter.value();
        }

        delete _instance;
        _instance = nullptr;
    }
}

PlanningManager* PlanningManager::instance ()
{
    if (_instance == nullptr)
        _instance = new PlanningManager ();
    return _instance;
}

void PlanningManager::startup()
{
    if(_state == State::Running) {
        qInfo () << "[PlanningManager] Routization still in progress";
        return;
    }

    if(_state != State::Initialized) {
        qInfo () << "[PlanningManager] PlanningManager not initialized";
        return;
    }

    _state = State::Running;

    /////////////////////////////////////
    ///         START TCP            ///
    /// ////////////////////////////////



    _state = State::Done;
}

void PlanningManager::stop()
{

}

bool PlanningManager:: loadObjects ()
{

    if (_isInitDone || _state == State::Initializing)
    {
        qInfo () << "[PlanningManager] Initialization done or still in progress";
        return false;
    }

    _state = State::Initializing;

    QVector<QObject *> const queue = loadQueue ();

    //-- Загрузка всех объектов из карты
    //--Все объекты выгружаются в порядке очереди(часть Дани Сажина)
    for (auto object : queue)
    {
        if (qobject_cast<Storage*>(object) != nullptr)
        {

            _objects.insert(qMakePair<double, double>(qobject_cast<Storage*>(object)->NE.first,
                                               qobject_cast<Storage*>(object)->NE.second), object);

            qInfo ().noquote () << QString ("[PlanningManager] Storage loaded successfully");
        }
        else if (qobject_cast<User*>(object) != nullptr)
        {

            _objects.insert(qMakePair<double, double>(qobject_cast<User*>(object)->NE.first,
                                               qobject_cast<User*>(object)->NE.second), object);

            qInfo ().noquote () << QString ("[PlanningManager] User loaded successfully");
        }
        else if (qobject_cast<Truck*>(object) != nullptr)
        {

            _objects.insert(qMakePair<double, double>(qobject_cast<Truck*>(object)->NE.first,
                                               qobject_cast<Truck*>(object)->NE.second), object);

            qInfo ().noquote () << QString ("[PlanningManager] Truck loaded successfully");
        }
        else
        {
            qCritical () << QString ("[PlanningManager] Object load failed");
        }
    }

    _isInitDone = true;

    emit signalComponentsLoaded ();

    return true;
}


void PlanningManager::init () {

    if(_state != State::Preparing) {
        return;
    }

    instance()->loadObjects ();
}

void PlanningManager::restart () {
    QTimer::singleShot (5 * 1000, this, &PlanningManager::signalRestartSystem);
}

QList<QObject*> PlanningManager::objects ()
{
    if (_instance == nullptr)
        return QList<QObject*> ();

    QList<QObject*> lst;
    QHashIterator<QPair<double, double>, QObject*> iter(_objects);
    while(iter.hasNext()) {
        iter.next();
        lst.append(iter.value());
    }

    return lst;
}

bool PlanningManager::isInitDone () const
{
    if (_instance == nullptr)
        return false;
    return true;
}

PlanningManager::State PlanningManager::state () const
{
    return _state;
}

QList<QObject*> PlanningManager::allObjects ()
{
    if (_instance != nullptr)
        return objects();

    return QList<QObject*> ();
}


bool PlanningManager::addObject (QObject *object)
{
    if(_instance == nullptr) {
        return false;
    }

    if (!object) {
        qWarning () << "[ComponentsCore] Trying to add null object";
        return false;
    }

    QWriteLocker locker (&_lock);
    if (qobject_cast<Storage*>(object) != nullptr)
    {

        _objects.insert(qMakePair<double, double>(qobject_cast<Storage*>(object)->NE.first,
                                           qobject_cast<Storage*>(object)->NE.second), object);

        qInfo ().noquote () << QString ("[PlanningManager] Storage loaded successfully");
    }
    else if (qobject_cast<User*>(object) != nullptr)
    {

        _objects.insert(qMakePair<double, double>(qobject_cast<User*>(object)->NE.first,
                                           qobject_cast<User*>(object)->NE.second), object);

        qInfo ().noquote () << QString ("[PlanningManager] User loaded successfully");
    }
    else if (qobject_cast<Truck*>(object) != nullptr)
    {

        _objects.insert(qMakePair<double, double>(qobject_cast<Truck*>(object)->NE.first,
                                           qobject_cast<Truck*>(object)->NE.second), object);

        qInfo ().noquote () << QString ("[PlanningManager] Truck loaded successfully");
    }
    else
    {
        qWarning () << QString ("[PlanningManager] Object load failed");
    }
    locker.unlock ();

    return true;
}

bool PlanningManager::removeObject (QObject* object)
{
    if(_instance == nullptr) {
        return false;
    }

    if (!object) {
        qWarning () << "[PlanningManager] Trying to remove null pointer object";
        return false;
    }

    QWriteLocker locker (&_lock);

    QHashIterator<QPair<double, double>, QObject*> iterator(_objects);

    if (object && qobject_cast<Storage*>(object) != nullptr)
    {

        Storage* _object = qobject_cast<Storage*>(object);

        while (iterator.hasNext()) {

            iterator.next();

            if(iterator.key().first == _object->NE.first && iterator.key().second == _object->NE.second) {

                delete iterator.value();
                _objects.remove(qMakePair(_object->NE.first, _object->NE.second));
                qInfo () << "[PlanningManager] remove Storage object succesfully";
                break;
            }
        }
    }
    else if (qobject_cast<User*>(object) != nullptr)
    {
        User* _object = qobject_cast<User*>(object);

        while (iterator.hasNext()) {

            iterator.next();

            if(iterator.key().first == _object->NE.first && iterator.key().second == _object->NE.second) {

                delete iterator.value();
                _objects.remove(qMakePair(_object->NE.first, _object->NE.second));
                qInfo () << "[PlanningManager] remove User object succesfully";
                break;
            }
        }
    }
    else if (qobject_cast<Truck*>(object) != nullptr)
    {
        Truck* _object = qobject_cast<Truck*>(object);

        while (iterator.hasNext()) {

            iterator.next();

            if(iterator.key().first == _object->NE.first && iterator.key().second == _object->NE.second) {

                delete iterator.value();
                _objects.remove(qMakePair(_object->NE.first, _object->NE.second));
                qInfo () << "[PlanningManager] remove Truck object succesfully";
                break;
            }
        }
    }
    else
    {
        qWarning () << QString ("[PlanningManager] Invalied object");
        return false;
    }

    locker.unlock ();

    return true;
}

bool PlanningManager::removeAllObject()
{
    if(_isFinishDone) {
        return false;
    }

    if (!_objects.isEmpty ())
            qInfo () << qPrintable (QString ("[PlanningManager] There are %1 objects left in the manager pool.")
                                    .arg (_objects.size ()));

    _objects.clear();
    return true;
}

void PlanningManager::shutdown ()
{
    if (_isFinishDone)
        return;

    _state = State::Stopping;

    if (!_objects.isEmpty ())
        qInfo () << qPrintable (QString ("[PlanningManager] There are %1 objects left in the manager pool.")
                                .arg (_objects.size ()));
    _isFinishDone = true;

    _state = State::Stopped;
}

QVector<QObject *> PlanningManager::loadQueue ()
{
    QVector<QObject *> queue;

    ///////////////////////////////////////////
    ///               САЖИН                 ///
    ///////////////////////////////////////////


    return queue;
}

void PlanningManager::slotComponentsLoaded()
{
    _state = State::Initialized;
}


