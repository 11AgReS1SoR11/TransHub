#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <QDataStream>
#include <QDebug>
#include "Aliases.hpp"

namespace Common
{

struct Coordinate
{
    double lat;
    double lon;

    bool operator==(const Coordinate& other) const
    {
        return (lat == other.lat) && (lon == other.lon);
    }
};

QDebug operator<<(QDebug debug, const Coordinate& coord);
QDataStream& operator<<(QDataStream& out, const Coordinate& coord);
QDataStream& operator>>(QDataStream& in, Coordinate& coord);

} // Common

#endif // COORDINATE_HPP