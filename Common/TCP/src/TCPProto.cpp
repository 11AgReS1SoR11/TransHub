#include "TCPProto.hpp"

namespace TCP::Protocol
{

std::unique_ptr<QString> Proto::getString() noexcept
{
    if (auto ptr = std::get_if<QString*>(&m_data))
    {
        return std::unique_ptr<QString>(*ptr);
    }

    return nullptr;
}

std::unique_ptr<Mtx::Matrix<int>> Proto::getMatrixInt() noexcept
{
    if (auto ptr = std::get_if<Mtx::Matrix<int>*>(&m_data))
    {
        return std::unique_ptr<Mtx::Matrix<int>>(*ptr);
    }

    return nullptr;
}

std::unique_ptr<Mtx::Matrix<double>> Proto::getMatrixDouble() noexcept
{
    if (auto ptr = std::get_if<Mtx::Matrix<double>*>(&m_data))
    {
        return std::unique_ptr<Mtx::Matrix<double>>(*ptr);
    }

    return nullptr;
}


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
    else
    {
        // TODO: some exception?
        debug << "No message";
    }

    return debug;
}

} // namespace TCP::Protocol