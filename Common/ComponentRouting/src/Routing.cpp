#include <string>
#include <sstream>
#include <httplib.h>

#include "Routing.hpp"

namespace Routing
{

std::string createRouteString(double fromLon, double fromLat, double toLon, double toLat)
{
    std::stringstream ss;
    ss << QUERY_ROUTE_OPEN_STREET_MAP_URL
       << fromLon << "," << fromLat << ";"
       << toLon << "," << toLat;
    return ss.str();
}

double GetRoute(Coordinate from, Coordinate to)
{
    std::string const dist = "distance";
    auto url = createRouteString(from.lon, from.lat, to.lon, to.lat);

    httplib::Client cli(OPEN_STREET_MAP_URL);
    auto res = cli.Get(url.c_str());

    if (res && res->status == 200)
    {
        auto idx_of_distance_start = res->body.find(dist);
        auto idx_of_distance_end = res->body.find("}", idx_of_distance_start);
        return std::stod(res->body.substr(idx_of_distance_start + dist.size() + 2, idx_of_distance_end));
    }

    return -1;
}

} // Routing