#include <vector>
#include <iostream>

namespace Algo
{

template<typename T>
using matrix = std::vector<std::vector<T>>;

template<typename T>
using vector = std::vector<T>;

template<typename T>
matrix<T> create_matrix(int rows, int cols, const T& initial_value = T())
{
    return matrix<T>(rows, std::vector<T>(cols, initial_value));
}

template<typename T>
struct Rout_and_length
{
    matrix<T> routes;
    T length = 999999999;
};

template<typename T>
void print_matrix(const matrix<T>& A)
{
    for (const auto& row : A)
    {
        for (const auto& elem : row)
        {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
bool CompareMatrix(const matrix<T>& matrix1, const matrix<T>& matrix2)
{
    if (matrix1.size() != matrix2.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < matrix1.size(); ++i)
    {
        if (matrix1[i].size() != matrix2[i].size())
        {
            return false;
        }

        for (std::size_t j = 0; j < matrix1[i].size(); ++j)
        {
            if (matrix1[i][j] != matrix2[i][j])
            {
                return false;
            }
        }
    }

    return true;
}

template<typename T>
bool is_all_zero(const vector<T>& A)
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
 * @param result_routes - current result of routers
 * @param g_min - global min (answer)
 * @param g_routes - global ruoutes (answer)
 */
template<typename T>
void Full_search_rec(vector<T>& storagesItems, vector<T>& clientsNeeds, vector<T>& couriersCapacity, const matrix<T>& roadsCouriersToStorages,
                        const matrix<T>& roadsStoragesToClients, T result, matrix<T>& result_routes, T& g_min, matrix<T>& g_routes)
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
                        result_routes.push_back({k + 1, j + 1, i + 1, roadsCouriersToStorages[j][i] + roadsStoragesToClients[k][j]});
                        auto d_old = couriersCapacity[k];
                        couriersCapacity[k] = 0;
                        storagesItems[j] -= 1;
                        clientsNeeds[i] -= 1;
                        Full_search_rec(storagesItems, clientsNeeds, couriersCapacity, roadsCouriersToStorages, roadsStoragesToClients, result, result_routes, g_min, g_routes);
                        couriersCapacity[k] = d_old;
                        storagesItems[j] += 1;
                        clientsNeeds[i] += 1;
                        result_routes.pop_back();
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
Rout_and_length<T> Full_search(vector<T>& storagesItems, vector<T>& clientsNeeds, vector<T>& couriersCapacity,
                                const matrix<T>& roadsCouriersToStorages, const matrix<T>& roadsStoragesToClients)
{
    matrix<T> result_routes;
    Rout_and_length<T> ans;
    Full_search_rec(storagesItems, clientsNeeds, couriersCapacity, roadsCouriersToStorages, roadsStoragesToClients, 0, result_routes, ans.length, ans.routes);
    return ans;
}

} // namespace Algo
