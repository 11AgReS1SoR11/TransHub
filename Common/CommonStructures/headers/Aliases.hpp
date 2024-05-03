#pragma once

namespace Common
{

struct Coordinate;

using Coordinates_t = QVector<Coordinate>;
using Capacity_t = QVector<int>;

enum class ObjectMapType
{
    Clients,
    Storages,
    Courier
};

} // Common
