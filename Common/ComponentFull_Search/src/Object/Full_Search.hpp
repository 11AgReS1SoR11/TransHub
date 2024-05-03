#pragma once

#include <QVector>
#include <QObject>
#include "Matrix.hpp"

namespace Algo
{

template<typename T>
struct Rout_and_length
{
    Mtx::Matrix<T> routes;
    T length = 999999999;
};

/**
 * Calculate the total distance traveled by couriers to deliver items to clients.
 *
 * @param storagesItems - items in storages
 * @param clientsNeeds - clients' needs
 * @param couriersCapacity - couriers' capacity
 * @param roadsCouriersToStorages - roads from couriers to storages
 * @param roadsStoragesToClients - roads from storages to clients
 * @returns struct with optimal routes and min road's length
 */
template<typename T>
Rout_and_length<T> Full_search(QVector<int>& storagesItems, QVector<int>& clientsNeeds, QVector<int>& couriersCapacity,
                                const Mtx::Matrix<T>& roadsCouriersToStorages, const Mtx::Matrix<T>& roadsStoragesToClients);

template<typename T>
class Full_Search : public QObject
{
public:
    static Full_Search& getInstance()
    {
        static Full_Search instance;
        return instance;
    }

    Rout_and_length<T> operator()(QVector<int>& storagesItems, QVector<int>& clientsNeeds, QVector<int>& couriersCapacity,
                    const Mtx::Matrix<T>& roadsCouriersToStorages, const Mtx::Matrix<T>& roadsStoragesToClients) const
    {
        return Full_search(storagesItems, clientsNeeds, couriersCapacity, roadsCouriersToStorages, roadsStoragesToClients);
    }

private:
    Full_Search() = default;
    ~Full_Search() = default;

    Full_Search(const Full_Search&) = delete;
    Full_Search& operator=(const Full_Search&) = delete;
};

template class Full_Search<int>;
template class Full_Search<double>;

} // namespace Algo
