#include "TCPProto.hpp"

namespace TCP::Protocol
{

template<typename T>
std::unique_ptr<T> Proto::get() noexcept
{
    if (auto ptr = std::get_if<T*>(&m_data))
    {
        auto uniquePtr = std::unique_ptr<T>(*ptr);
        std::visit([](auto& data){ data = nullptr; }, m_data);
        return uniquePtr;
    }

    return nullptr;
}

template std::unique_ptr<Common::Capacity_t> Proto::get<Common::Capacity_t>() noexcept;
template std::unique_ptr<Common::Coordinates_t> Proto::get<Common::Coordinates_t>() noexcept;
template std::unique_ptr<QString> Proto::get<QString>() noexcept;
template std::unique_ptr<Mtx::Matrix<int>> Proto::get<Mtx::Matrix<int>>() noexcept;
template std::unique_ptr<Mtx::Matrix<double>> Proto::get<Mtx::Matrix<double>>() noexcept;

QDataStream& operator<<(QDataStream& out, const Proto& proto)
{
    static auto writeMatrix = [](QDataStream& stream, auto const* matrix_ptr) {
        auto const& matrix = *matrix_ptr;
        stream << matrix.rows() << matrix.columns();
        for (int i = 0; i < matrix.rows(); ++i)
        {
            for (int j = 0; j < matrix.columns(); ++j)
            {
                stream << matrix[i][j];
            }
        }
    };

    if (auto data_ptr = std::get_if<QString*>(&proto.m_data))
    {
        out << static_cast<int>(proto.m_data.index()); // write type
        auto const* string_ptr = *data_ptr;
        out << *string_ptr;
    }
    else if (auto data_ptr = std::get_if<Mtx::Matrix<int>*>(&proto.m_data))
    {
        out << static_cast<int>(proto.m_data.index()); // write type
        writeMatrix(out, *data_ptr);
    }
    else if (auto data_ptr = std::get_if<Mtx::Matrix<double>*>(&proto.m_data))
    {
        out << static_cast<int>(proto.m_data.index()); // write type
        writeMatrix(out, *data_ptr);
    }
    else if (auto data_ptr = std::get_if<Common::Coordinates_t*>(&proto.m_data))
    {
        out << static_cast<int>(proto.m_data.index()); // write type
        auto const* coordinates_ptr = *data_ptr;
        auto const& coordinates = *coordinates_ptr;
        out << coordinates.size();
        for (int j = 0; j < coordinates.size(); ++j)
        {
            out << coordinates[j];
        }
    }
    else if (auto data_ptr = std::get_if<Common::Capacity_t*>(&proto.m_data))
    {
        out << static_cast<int>(proto.m_data.index()); // write type
        auto const* capacity_ptr = *data_ptr;
        auto const& capacity = *capacity_ptr;
        out << capacity.size();
        for (int j = 0; j < capacity.size(); ++j)
        {
            out << capacity[j];
        }
    }
    else
    {
        // TODO: some exception?
        qDebug() << "[Error][TCPProto] unknown type";
    }

    return out;
}

QDataStream& operator>>(QDataStream& in, Proto& proto)
{
    static auto readMatrix = [](QDataStream& stream, auto* matrix_ptr) {
        auto& matrix = *matrix_ptr;
        for (int i = 0; i < matrix.rows(); ++i)
        {
            for (int j = 0; j < matrix.columns(); ++j)
            {
                stream >> matrix[i][j];
            }
        }
    };

    int dataTypeIndex;
    in >> dataTypeIndex;

    if (dataTypeIndex == static_cast<int>(Proto::DataTypeIndex::MatrixInt))
    {
        int rows, columns;
        in >> rows >> columns;
        auto* matrix = new Mtx::Matrix<int>(rows, columns);
        readMatrix(in, matrix);
        proto.m_data = matrix;
    }
    else if (dataTypeIndex == static_cast<int>(Proto::DataTypeIndex::MatrixDouble))
    {
        int rows, columns;
        in >> rows >> columns;
        auto* matrix = new Mtx::Matrix<double>(rows, columns);
        readMatrix(in, matrix);
        proto.m_data = matrix;
    }
    else if (dataTypeIndex == static_cast<int>(Proto::DataTypeIndex::String))
    {
        QString* str = new QString();
        in >> *str;
        proto.m_data = str;
    }
    else if (dataTypeIndex == static_cast<int>(Proto::DataTypeIndex::Coordinates))
    {
        int size;
        in >> size;
        Common::Coordinates_t* coordinates = new Common::Coordinates_t(); // TODO: size
        for (int j = 0; j < size; ++j)
        {
            Common::Coordinate coordinate;
            in >> coordinate;
            coordinates->push_back(coordinate);
        }
        proto.m_data = coordinates;
    }
    else if (dataTypeIndex == static_cast<int>(Proto::DataTypeIndex::Capacity))
    {
        int size;
        in >> size;
        Common::Capacity_t* capacity = new Common::Capacity_t();
        for (int j = 0; j < size; ++j)
        {
            int cap;
            in >> cap;
            capacity->push_back(cap);
        }
        proto.m_data = capacity;
    }
    else
    {
        // TODO: some exception?
        qDebug() << "[Error][TCPProto] unknown type";
    }

    return in;
}

QDebug operator<<(QDebug debug, const Proto& proto)
{
    if (auto data_ptr = std::get_if<QString*>(&proto.m_data))
    {
        auto const* string_ptr = *data_ptr;
        debug << *string_ptr;
    }
    else if (auto data_ptr = std::get_if<Mtx::Matrix<int>*>(&proto.m_data))
    {
        auto const* matrix_ptr = *data_ptr;
        matrix_ptr->print();
    }
    else if (auto data_ptr = std::get_if<Mtx::Matrix<double>*>(&proto.m_data))
    {
        auto const* matrix_ptr = *data_ptr;
        matrix_ptr->print();
    }
    else if (auto data_ptr = std::get_if<Common::Coordinates_t*>(&proto.m_data))
    {
        auto const* coordinates_ptr = *data_ptr;
        auto const& coordinates = *coordinates_ptr;
        for (int j = 0; j < coordinates.size(); ++j)
        {
            debug << coordinates[j] << " ";
        }
    }
    else if (auto data_ptr = std::get_if<Common::Capacity_t*>(&proto.m_data))
    {
        auto const* capacity_ptr = *data_ptr;
        auto const& capacity = *capacity_ptr;
        for (int j = 0; j < capacity.size(); ++j)
        {
            debug << capacity[j] << " ";
        }
    }
    else
    {
        // TODO: some exception?
        debug << "No message";
    }

    return debug;
}

} // namespace TCP::Protocol