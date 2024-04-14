#ifndef TCPProto_HPP
#define TCPProto_HPP

#include <QDataStream>
#include "Matrix.hpp"
#include <variant>
#include <QString>
#include <memory>

namespace TCP::Protocol
{

class Proto
{
public:
    Proto(Mtx::Matrix<int>* data) : m_data(data) {}
    Proto(Mtx::Matrix<double>* data) : m_data(data) {}
    Proto(QString* data) : m_data(data) {}
    Proto() {}

    std::unique_ptr<QString> getString() noexcept;
    std::unique_ptr<Mtx::Matrix<int>> getMatrixInt() noexcept;
    std::unique_ptr<Mtx::Matrix<double>> getMatrixDouble() noexcept;

    friend QDataStream& operator<<(QDataStream& out, const Proto& proto);
    friend QDataStream& operator>>(QDataStream& in, Proto& proto);

private:
    enum class DataTypeIndex { MatrixInt = 0, MatrixDouble = 1, String = 2 }; // mb short?
    using DataType = std::variant<Mtx::Matrix<int>*, Mtx::Matrix<double>*, QString*>;
    DataType m_data;
};

} // namespace TCP::Protocol

#endif // TCPProto_HPP
