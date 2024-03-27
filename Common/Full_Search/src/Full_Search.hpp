#include <vector>

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
Rout_and_length<T> Full_search(std::vector<T>& storagesItems, std::vector<T>& clientsNeeds, std::vector<T>& couriersCapacity,
                                const Mtx::Matrix<T>& roadsCouriersToStorages, const Mtx::Matrix<T>& roadsStoragesToClients);

} // namespace Algo
