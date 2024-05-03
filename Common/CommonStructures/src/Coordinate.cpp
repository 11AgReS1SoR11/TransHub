#include "Coordinate.hpp"

namespace Common
{

QDebug operator<<(QDebug debug, const Coordinate& coord)
{
    debug << "Coordinate(" << coord.lat << ", " << coord.lon << ")";
    return debug;
}

QDataStream& operator<<(QDataStream& out, const Coordinate& coord)
{
    out << coord.lat << coord.lon;
    return out;
}

QDataStream& operator>>(QDataStream& in, Coordinate& coord)
{
    in >> coord.lat >> coord.lon;
    return in;
}

} // Common