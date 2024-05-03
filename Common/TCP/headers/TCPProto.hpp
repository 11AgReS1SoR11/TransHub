#ifndef TCPProto_HPP
#define TCPProto_HPP

#include <QDataStream>
#include "Matrix.hpp"
#include "Coordinate.hpp"
#include <variant>
#include <QString>
#include <memory>

namespace TCP::Protocol
{

class Proto
{

public:
    template<typename T>
    Proto(T* data) : m_data(data) {}
    Proto() {}

    template<typename T>
    std::unique_ptr<T> get() noexcept;

    friend QDataStream& operator<<(QDataStream& out, const Proto& proto);
    friend QDataStream& operator>>(QDataStream& in, Proto& proto);

    friend QDebug operator<<(QDebug debug, const Proto& proto);

private:
    enum class DataTypeIndex { MatrixInt, MatrixDouble, String, Coordinates, Capacity };
    using DataType = std::variant<Mtx::Matrix<int>*, Mtx::Matrix<double>*, QString*, Common::Coordinates_t*, Common::Capacity_t*>;
    DataType m_data;
};

} // namespace TCP::Protocol

#endif // TCPProto_HPP
