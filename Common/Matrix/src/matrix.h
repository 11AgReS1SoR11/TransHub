#ifndef MATRIX_H
#define MATRIX_H

//#include "Matrix_global.h"
#include <vector>
#include <iostream>
namespace Mtx{
template <typename T>
class Matrix
{
public:

    Matrix() = default;

    Matrix(const Matrix& mat) = default;

    Matrix(std::vector<std::vector<T>>& vec);

    Matrix(T** arr);

    Matrix(std::vector<std::vector<T>>&& vec);

    std::vector<T>& operator [] (int i);

    bool operator == (const Matrix& other_matrix) const;

    Matrix operator * (const Matrix& other_matrix) const;

    Matrix operator + (const Matrix& other_matrix) const;

    Matrix concatenation(const Matrix& other_matrix) const;

    Matrix& operator = (const Matrix& other_matrix) const noexcept;

    double determinant() const;

    void print() const noexcept;

    static void print(const Matrix& matrix) noexcept;


private:

    std::vector<std::vector<T>> _data;
    int _rows;
    int _columns;

    bool match_sizes(const Matrix<T>& first_matrix, const Matrix<T>& second_matrix) const noexcept;
    static double det(const std::vector<std::vector<T>>& matrix);
    static std::vector<std::vector<T>> minor(const std::vector<std::vector<T>>& matrix, int row, int col);
};

template<typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>>& vec) {
    int row_size = vec[0].size();
    for (const auto& tmp :vec){
        if (tmp.size() != static_cast<std::size_t>(row_size))
            throw std::runtime_error("Incorrect vector of vectors");
    }
    _data = vec;
    _rows = vec.size(); 
    _columns = vec[0].size();
}

template<typename T>
Matrix<T>::Matrix(T** arr): _rows(sizeof(arr) / sizeof(T) / sizeof(arr[0])), _columns(sizeof(arr[0]) / sizeof(T)){
    _data = std::vector<std::vector<T>> (_rows, std::vector<T>(_columns));

    for (int i = 0; i < _rows; ++i)
        for (int j = 0; j < _columns; ++j)
            _data[i][j] = arr[i][j];
}

template <typename T>
Matrix<T>::Matrix(std::vector<std::vector<T>>&& vec): _data(std::move(vec)), _rows(_data.size()), _columns(_data[0].size()){}

template <typename T>
std::vector<T>& Matrix<T>::operator [] (int i){
    return _data[i];
}


template <typename T>
bool Matrix<T>::operator == (const Matrix<T>& other_matrix) const{

    if (!match_sizes(*this, other_matrix))
        throw std::runtime_error("Matrix sizes don't match");

    for (int i = 0; i < _rows; ++i)
        for (int j = 0; j < _columns; ++j)
            if (_data[i][j] != other_matrix._data[i][j])
                return false;
    return true;
}

template <typename T>
Matrix<T> Matrix<T>::operator * (const Matrix& other_matrix) const{
    if (this->_columns != other_matrix._rows)
        throw std::runtime_error("Number of columns from first matrix is not equal to number of rows from second matrix");
    std::vector<std::vector<T>> ans(_rows, std::vector<T>(other_matrix._columns, 0));
    for (int i = 0; i < _rows; ++i){
        for (int j = 0; j < other_matrix._columns; ++j){
            for (int k = 0; k < _columns; ++k)
                ans[i][j] += _data[i][k] * other_matrix._data[k][j];
        }
    }
    return Matrix(std::move(ans));
}

template <typename T>
Matrix<T> Matrix<T>::operator + (const Matrix& other_matrix) const{
    if (!match_sizes(*this, other_matrix))
        throw std::runtime_error("Matrix sizes don't match");
    std::vector<std::vector<T>> ans(_rows, std::vector<T>(_columns));
    for (int i = 0; i < _rows; ++i)
        for (int j = 0; j < _columns; ++j)
            ans[i][j] = _data[i][j] + other_matrix._data[i][j];
    return Matrix(std::move(ans));
}

template <typename T>
Matrix<T>& Matrix<T>::operator = (const Matrix& other_matrix) const noexcept{
    if (other_matrix != *this){
        _data = other_matrix._data;
        _rows = other_matrix._rows;
        _columns = other_matrix._columns;
    }
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::concatenation(const Matrix& other_matrix) const{
    if (_rows != other_matrix._rows)
        throw std::runtime_error("Row sizes don`t match");
    std::vector<std::vector<T>> ans(_rows , std::vector<T>(_columns + other_matrix._columns));

    for (int i = 0; i < _rows; ++i){
        for (int j = 0; j < _columns; ++j){
            ans[i][j] = _data[i][j];
        }
    }

    for (int i = 0; i < other_matrix._rows; ++i){
        for (int j = 0; j < other_matrix._columns; ++j){
            ans[i][j+_columns] = other_matrix._data[i][j];
        }
    }
    return Matrix(std::move(ans));
}

template <typename T>
double Matrix<T>::determinant() const{
    if (_rows != _columns)
        throw std::runtime_error("Matrix is not square");
    return Matrix<T>::det(this->_data);
}

template <typename T>
void Matrix<T>::print() const noexcept{
    std::cout << "Data:\n";
    for (int i = 0; i < _rows; ++i){
        for (int j = 0; j < _columns; ++j)
            std::cout << _data[i][j] << ' ';
        std::cout << std::endl;
    }
    std::cout << "Size: number of rows " << _rows << ", numbers of columns " << _columns << std::endl;
}

template <typename T>
bool Matrix<T>::match_sizes(const Matrix<T>& first_matrix, const Matrix<T>& second_matrix) const noexcept{
    if (first_matrix._rows != second_matrix._rows || first_matrix._columns != second_matrix._columns)
        return false;
    return true;
}

template <typename T>
double Matrix<T>::det(const std::vector<std::vector<T>>& matrix) {
    int n = matrix.size(); // Размерность матрицы

    // Базовый случай: если размер матрицы 1x1, то определитель равен этому элементу
    if (n == 1) {
        return matrix[0][0];
    }

    double dett = 0; // Переменная для хранения определителя

    // Вычисление определителя по первой строке (или по любой другой)
    for (int j = 0; j < n; ++j) {
        // Вычисляем минор для элемента matrix[0][j]
        std::vector<std::vector<T>> minor_matrix = minor(matrix, 0, j);

        // Рекурсивно вычисляем определитель для минора и его алгебраическое дополнение
        double minor_det = Matrix<T>::det(minor_matrix);
        double algebraic_cofactor = (j % 2 == 0) ? matrix[0][j] * minor_det : -matrix[0][j] * minor_det;

        // Суммируем алгебраические дополнения
        dett += algebraic_cofactor;
    }

    return dett;
}

template <typename T>
std::vector<std::vector<T>> Matrix<T>::minor(const std::vector<std::vector<T>>& matrix, int row, int col) {
    std::vector<std::vector<T>> result;
    for (std::size_t i = 0; i < matrix.size(); ++i) {
        if (i != static_cast<std::size_t>(row)) {
            std::vector<T> temp;
            for (std::size_t j = 0; j < matrix[i].size(); ++j) {
                if (j != static_cast<std::size_t>(col)) {
                    temp.push_back(matrix[i][j]);
                }
            }
            result.push_back(temp);
        }
    }
    return result;
}

template <typename T>
void Matrix<T>::print(const Matrix& matrix) noexcept{
    matrix.print();
}
} // namespace Matrix
#endif // MATRIX_H
