#pragma once

#include <QObject>
#include <QDebug>
#include <memory>
#include "TCPServer.hpp"
#include "TCPProto.hpp"

// TODO: parse port from args line
inline TCP::port_t PORT = 9090;

class ServiceManager : public QObject
{
    Q_OBJECT

public:
    explicit ServiceManager(QObject* parent = nullptr);
    ~ServiceManager();

signals:
    void dataCollected(TCP::tcp_id_t clientId);
    void algoCompleted(TCP::tcp_id_t clientId, Mtx::Matrix<double>& routes, double length);
    void algoFailed(TCP::tcp_id_t clientId, QString& errorMsg);

private slots:
    void notifyClient(TCP::tcp_id_t clientId, Mtx::Matrix<double>& routes, double length);
    void notifyFailClient(TCP::tcp_id_t clientId, QString& errorMsg);
    void processAlgo(TCP::tcp_id_t clientId);
    void getData(TCP::tcp_id_t clientId, TCP::Protocol::Proto& proto);

private:
    bool checkDataCollected();
    void reset();

    TCP::TCPServer m_Server{PORT};
    std::unique_ptr<Common::Coordinates_t> m_Clients;
    std::unique_ptr<Common::Coordinates_t> m_Storages;
    std::unique_ptr<Common::Coordinates_t> m_Courier;
    std::unique_ptr<Common::Capacity_t> m_ClientsCapacity;
    std::unique_ptr<Common::Capacity_t> m_StoragesCapacity;
    std::unique_ptr<Common::Capacity_t> m_CourierCapacity;
    Common::ObjectMapType m_nextCoordinateDataType;
    Common::ObjectMapType m_nextCapacityDataType;
};

