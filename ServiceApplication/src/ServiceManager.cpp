#include "ServiceManager.hpp"
#include "Routing.hpp"
#include "Full_Search.hpp"
#include <QTimer>

ServiceManager::ServiceManager(QObject* parent) : QObject(parent)
{
    connect(&m_Server, &TCP::TCPServer::newMessage, this, &ServiceManager::getData);
    connect(this, &ServiceManager::dataCollected, this, &ServiceManager::processAlgo);
    connect(this, &ServiceManager::algoCompleted, this, &ServiceManager::notifyClient);
}

ServiceManager::~ServiceManager() {}

void ServiceManager::notifyClient(TCP::tcp_id_t clientId, Mtx::Matrix<double>& routes, double length)
{
    m_Server.sendMessage(clientId, &routes);

    // TODO: сделать нормально TRH-84
    QTimer::singleShot(10, this, [this, clientId, length]() {
        QString len = QString::number(length);
        m_Server.sendMessage(clientId, &len);
    });
}

void ServiceManager::processAlgo(TCP::tcp_id_t clientId)
{
    Mtx::Matrix<double> C(m_Storages->size(), m_Clients->size());
    for (int i = 0; i < C.rows(); ++i)
    {
        for (int j = 0; j < C.columns(); ++j)
        {
            C[i][j] = Routing::GetRoute((*m_Storages)[i], (*m_Clients)[j]);
        }
    }

    Mtx::Matrix<double> D(m_Courier->size(), m_Storages->size());
    for (int i = 0; i < D.rows(); ++i)
    {
        for (int j = 0; j < D.columns(); ++j)
        {
            D[i][j] = Routing::GetRoute((*m_Courier)[i], (*m_Storages)[j]);
        }
    }

    auto [routes, length] = Algo::Full_search(*m_StoragesCapacity, *m_ClientsCapacity, *m_CourierCapacity, C, D);
    emit algoCompleted(clientId, routes, length);
}

void ServiceManager::getData(TCP::tcp_id_t clientId, TCP::Protocol::Proto proto)
{
    if (auto string_ptr = proto.get<QString>())
    {
        auto const& msg = *string_ptr;
        if (msg == "Clients")
        {
            m_nextCoordinateDataType = Common::ObjectMapType::Clients;
        }
        else if (msg == "Storages")
        {
            m_nextCoordinateDataType = Common::ObjectMapType::Storages;
        }
        else if (msg == "Couriers")
        {
            m_nextCoordinateDataType = Common::ObjectMapType::Courier;
        }
        else if (msg == "Clients capacity")
        {
            m_nextCapacityDataType = Common::ObjectMapType::Clients;
        }
        else if (msg == "Storages capacity")
        {
            m_nextCapacityDataType = Common::ObjectMapType::Storages;
        }
        else if (msg == "Couriers capacity")
        {
            m_nextCapacityDataType = Common::ObjectMapType::Courier;
        }
        else
        {
            qDebug() << msg;
        }
    }
    else if (auto coordinates_ptr = proto.get<Common::Coordinates_t>())
    {
        switch (m_nextCoordinateDataType)
        {
            case Common::ObjectMapType::Clients:
                m_Clients = std::move(coordinates_ptr);
                break;
            case Common::ObjectMapType::Storages:
                m_Storages = std::move(coordinates_ptr);
                break;
            case Common::ObjectMapType::Courier:
                m_Courier = std::move(coordinates_ptr);
                break;
            default:
                // TODO: exception?
                break;
        }
    }
    else if (auto capacity_ptr = proto.get<Common::Capacity_t>())
    {
        switch (m_nextCapacityDataType)
        {
            case Common::ObjectMapType::Clients:
                m_ClientsCapacity = std::move(capacity_ptr);
                break;
            case Common::ObjectMapType::Storages:
                m_StoragesCapacity = std::move(capacity_ptr);
                break;
            case Common::ObjectMapType::Courier:
                m_CourierCapacity = std::move(capacity_ptr);
                break;
            default:
                // TODO: exception?
                break;
        }
    }   
    else
    {
        qDebug() << "[ServiceManager] Recieve unexpected message";
    }

    if (checkDataCollected())
    {
        emit dataCollected(clientId);
    }
}

bool ServiceManager::checkDataCollected()
{
    if (m_Clients && m_Storages && m_Courier && m_ClientsCapacity && m_StoragesCapacity && m_CourierCapacity)
    {
        if (m_Clients->isEmpty() || m_Storages->isEmpty() || m_Courier->isEmpty() ||
            m_ClientsCapacity->isEmpty() || m_StoragesCapacity->isEmpty() || m_CourierCapacity->isEmpty())
        {
            return false;
        }

        return true;
    }

    return false;
}
