#include "Constants.hpp"

namespace Routing
{

struct Coordinate
{
    double lat;
    double lon;
};

double GetRoute(Coordinate from, Coordinate to);

} // Routing
