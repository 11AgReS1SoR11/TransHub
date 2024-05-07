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
#include <QtTest/QtTest>
#include <random>

#include <algorithm>
#include <iostream>

#define PORT 9090

using namespace Planning;

PlanningManager* PlanningManager::_instance = nullptr;

QHash<QPair<double, double>, QObject*> PlanningManager:: _objects = QHash<QPair<double, double>, QObject*> ();

QList<QObject*> PlanningManager:: lst_objects = QList<QObject*> ();

QReadWriteLock PlanningManager:: _lock(QReadWriteLock::RecursionMode::NonRecursive);

bool PlanningManager:: _isFinishDone = false;

bool PlanningManager:: _isInitDone = false;

TCP::TCPClient PlanningManager:: client(PORT);

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
        removeAllObject();
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

//    if(_state != State::Initialized) {
//        qInfo () << "[PlanningManager] PlanningManager not initialized";
//        return;
//    }

    _state = State::Running;

    /////////////////////////////////////
    ///         START TCP            ///
    /// ////////////////////////////////

    bool connected = client.connectToHost(PORT);


    //доделать
    if(connected == true) {

    }
    else {

    }

    connect(&client, &TCP::TCPClient::newMessage, this, &Planning::PlanningManager::slotTCP);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 20);

    QVector<int> StoragesCapacity;
    QVector<int> ClientsCapacity;
    QVector<int> CouriersCapacity;

    Common::Coordinates_t CouriersCoordinates;
    Common::Coordinates_t StoragesCoordinates;
    Common::Coordinates_t ClientsCoordinates ;

    for(auto object : allObjects()) {

        if (auto* obj = qobject_cast<Planning::Storage*>(object))
        {

            const Common::Coordinate storage = {.lat = obj->NE.first,
                                                .lon = obj->NE.second};

            StoragesCoordinates.push_back(storage);

        }
        else if (auto* obj = qobject_cast<Planning::User*>(object))
        {

            const Common::Coordinate client = {.lat = obj->NE.first,
                                             .lon = obj->NE.second};

            ClientsCoordinates.push_back(client);
        }
        else if (auto* obj = qobject_cast<Planning::Truck*>(object))
        {

            const Common::Coordinate courier = {.lat = obj->NE.first,
                                             .lon = obj->NE.second};

            CouriersCoordinates.push_back(courier);
        }
    }

    for (int i = 0; i < StoragesCoordinates.size(); ++i) {
        StoragesCapacity.append(dis(gen));
    }

    for(int i = 0; i < ClientsCoordinates.size(); ++i) {
        ClientsCapacity.append(dis(gen));
    }

    for(int i = 0; i < CouriersCoordinates.size(); ++i) {
        CouriersCapacity.append(1);
    }

    QTest::qWait(100);
    QString msg1 = "Clients";
    client.sendMessage(&msg1);
    QTest::qWait(100);
    client.sendMessage(&ClientsCoordinates);

    QTest::qWait(100);
    QString msg2 = "Storages";
    client.sendMessage(&msg2);
    QTest::qWait(100);
    client.sendMessage(&StoragesCoordinates);

    QTest::qWait(100);
    QString msg3 = "Couriers";
    client.sendMessage(&msg3);
    QTest::qWait(100);
    client.sendMessage(&CouriersCoordinates);

    QTest::qWait(100);
    QString msg4 = "Clients capacity";
    client.sendMessage(&msg4);
    QTest::qWait(100);
    client.sendMessage(&ClientsCapacity);

    QTest::qWait(100);
    QString msg5 = "Storages capacity";
    client.sendMessage(&msg5);
    QTest::qWait(100);
    client.sendMessage(&StoragesCapacity);

    QTest::qWait(100);
    QString msg6 = "Couriers capacity";
    client.sendMessage(&msg6);
    QTest::qWait(100);
    client.sendMessage(&CouriersCapacity);

    _state = State::Done;
}

void PlanningManager::stop()
{
    client.disconnect();
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

    return lst_objects;
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

    lst_objects.append(object);

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

                for (auto it = lst_objects.begin(); it != lst_objects.end(); ++it) {
                    if(*it == iterator.value()) {
                        lst_objects.erase(it);
                        break;
                    }
                }

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

                for (auto it = lst_objects.begin(); it != lst_objects.end(); ++it) {
                    if(*it == iterator.value()) {
                        lst_objects.erase(it);
                        break;
                    }
                }

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

                for (auto it = lst_objects.begin(); it != lst_objects.end(); ++it) {
                    if(*it == iterator.value()) {
                        lst_objects.erase(it);
                        break;
                    }
                }

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

    QHashIterator<QPair<double, double>, QObject*> iter(_objects);
    while(iter.hasNext()) {
        iter.next();
        delete iter.value();
    }

    _objects.clear();
    lst_objects.clear();
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
    ///   На потом реализовать часть Дани   ///
    ///////////////////////////////////////////


    return queue;
}

void PlanningManager::slotComponentsLoaded()
{
    _state = State::Initialized;
}

void PlanningManager::slotTCP(TCP::Protocol::Proto proto)
{

    static bool cnt = 0;

    if(cnt == false) {
        cnt = true;
    }
    else {
        cnt = false;
        emit breakConnection(QIcon::fromTheme("media-playback-start"));
    }

    if (auto string_ptr = proto.get<QString>())
    {
        auto const& msg = *string_ptr;
        double answer = msg.toDouble();
        qDebug() << answer;
    }
    else if (auto matrix_ptr = proto.get<Mtx::Matrix<double>>())
    {
        auto const& matrix_received = *matrix_ptr;
        //matrix_received.print();

        Common::Coordinates_t CouriersCoordinates;
        Common::Coordinates_t StoragesCoordinates;
        Common::Coordinates_t ClientsCoordinates ;

        for(auto object : allObjects()) {

            if (auto* obj = qobject_cast<Planning::Storage*>(object))
            {

                const Common::Coordinate storage = {.lat = obj->NE.first,
                                                    .lon = obj->NE.second};

                StoragesCoordinates.push_back(storage);

            }
            else if (auto* obj = qobject_cast<Planning::User*>(object))
            {

                const Common::Coordinate client = {.lat = obj->NE.first,
                                                 .lon = obj->NE.second};

                ClientsCoordinates.push_back(client);
            }
            else if (auto* obj = qobject_cast<Planning::Truck*>(object))
            {

                const Common::Coordinate courier = {.lat = obj->NE.first,
                                                 .lon = obj->NE.second};

                CouriersCoordinates.push_back(courier);
            }
        }

        emit startPlotting(CouriersCoordinates, StoragesCoordinates, ClientsCoordinates, matrix_received);

    }
    else
    {
        qWarning() << proto;
    }
}


