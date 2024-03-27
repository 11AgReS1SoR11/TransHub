#include "Full_Search.hpp"

namespace Algo
{

template<typename T>
bool is_all_zero(const QVector<T>& A)
{
    for (const auto& i : A)
    {
        if (i != 0) return false;
    }

    return true;
}

/**
 * Calculate the total distance traveled by couriers to deliver items to clients.
 *
 * @param storagesItems - items in storages
 * @param clientsNeeds - clients' needs
 * @param couriersCapacity - couriers' capacity
 * @param roadsCouriersToStorages - roads from couriers to storages
 * @param roadsStoragesToClients - roads from storages to clients
 * @param result - current result
 * @param result_routes - current result of routers
 * @param g_min - global min (answer)
 * @param g_routes - global ruoutes (answer)
 */
template<typename T>
void Full_search_rec(QVector<T>& storagesItems, QVector<T>& clientsNeeds, QVector<T>& couriersCapacity, const Mtx::Matrix<T>& roadsCouriersToStorages,
                        const Mtx::Matrix<T>& roadsStoragesToClients, T result, Mtx::Matrix<T>& result_routes, T& g_min, Mtx::Matrix<T>& g_routes)
{
    if (is_all_zero(storagesItems) || is_all_zero(clientsNeeds) || is_all_zero(couriersCapacity))
    {
        if (result < g_min)
        {
            g_min = result;
            g_routes = result_routes;
            return;
        }
    }

    if (result > g_min)
    {
        return;
    }

    int m = storagesItems.size();
    int n = clientsNeeds.size();
    int p = couriersCapacity.size();

    for (int k = 0; k < p; ++k)
    {
        if (couriersCapacity[k] != 0)
        {
            for (int j = 0; j < m; ++j)
            {
                if (storagesItems[j] != 0)
                {
                    for (int i = 0; i < n; ++i)
                    {
                        result += roadsCouriersToStorages[j][i] + roadsStoragesToClients[k][j];
                        result_routes.addRow({k + 1, j + 1, i + 1, roadsCouriersToStorages[j][i] + roadsStoragesToClients[k][j]});
                        auto d_old = couriersCapacity[k];
                        couriersCapacity[k] = 0;
                        storagesItems[j] -= 1;
                        clientsNeeds[i] -= 1;
                        Full_search_rec(storagesItems, clientsNeeds, couriersCapacity, roadsCouriersToStorages, roadsStoragesToClients, result, result_routes, g_min, g_routes);
                        couriersCapacity[k] = d_old;
                        storagesItems[j] += 1;
                        clientsNeeds[i] += 1;
                        result_routes.popRow();
                        result -= roadsCouriersToStorages[j][i] + roadsStoragesToClients[k][j];
                    }
                }
            }
        }
    }
}

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
Rout_and_length<T> Full_search(QVector<T>& storagesItems, QVector<T>& clientsNeeds, QVector<T>& couriersCapacity,
                                const Mtx::Matrix<T>& roadsCouriersToStorages, const Mtx::Matrix<T>& roadsStoragesToClients)
{
    Mtx::Matrix<T> result_routes;
    Rout_and_length<T> ans;
    Full_search_rec(storagesItems, clientsNeeds, couriersCapacity, roadsCouriersToStorages, roadsStoragesToClients, T(), result_routes, ans.length, ans.routes);
    return ans;
}

template Rout_and_length<int> Full_search(QVector<int>& storagesItems, QVector<int>& clientsNeeds, QVector<int>& couriersCapacity,
                                const Mtx::Matrix<int>& roadsCouriersToStorages, const Mtx::Matrix<int>& roadsStoragesToClients);
template Rout_and_length<double> Full_search(QVector<double>& storagesItems, QVector<double>& clientsNeeds, QVector<double>& couriersCapacity,
                                const Mtx::Matrix<double>& roadsCouriersToStorages, const Mtx::Matrix<double>& roadsStoragesToClients);
template void Full_search_rec(QVector<int>& storagesItems, QVector<int>& clientsNeeds, QVector<int>& couriersCapacity, const Mtx::Matrix<int>& roadsCouriersToStorages,
                        const Mtx::Matrix<int>& roadsStoragesToClients, int result, Mtx::Matrix<int>& result_routes, int& g_min, Mtx::Matrix<int>& g_routes);
template void Full_search_rec(QVector<double>& storagesItems, QVector<double>& clientsNeeds, QVector<double>& couriersCapacity, const Mtx::Matrix<double>& roadsCouriersToStorages,
                        const Mtx::Matrix<double>& roadsStoragesToClients, double result, Mtx::Matrix<double>& result_routes, double& g_min, Mtx::Matrix<double>& g_routes);
template bool is_all_zero(const QVector<int>& A);
template bool is_all_zero(const QVector<double>& A);

} // namespace Algo
