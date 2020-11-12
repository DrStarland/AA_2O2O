#ifndef MATRIX_OPERATIONS_HPP
#define MATRIX_OPERATIONS_HPP

#include "matrix.h"

#include <thread>
#include <functional>

template <typename T>
Matrix<T>::Matrix(size_t N) : Matrix(N, N) {};
template <typename T>
Matrix<T>::Matrix(size_t n, size_t m) : Matrix(n, m, nullptr, 0) {};
template <typename T>
Matrix<T>::Matrix(initializer_list<initializer_list<T>> list) : Matrix(list.size(), list.begin()[0].size()) {
    this->makeCopyFrom(list);
}
template <typename T>
Matrix<T>::Matrix(const Matrix<T> &copy) : Matrix(copy.m_rows, copy.m_columns, &copy, 1) {}
template <typename T>
Matrix<T>::Matrix(Matrix<T> &&copy) noexcept { this->_move(move(copy)); }
template <typename T>
Matrix<T>::Matrix(size_t n, size_t m, const Matrix<T> *source, unsigned char mode) : BasicContainer() {
    allocMatrix(n, m);
    if (mode) this->makeCopyFrom(*source);
}
template <typename T>
Matrix<T>::Matrix(size_t n, size_t m, T** ptr) : Matrix(n, m) {
    { time_t _time = time(NULL);
    if (ptr == nullptr)
        throw PointerInitException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time)); }
    for (size_t i = 0; i < this->m_rows; i++)
        for (size_t j = 0; j < this->m_columns; j++)
            (*this)[i][j] = ptr[i][j];
}
template <typename T> template <typename V>
Matrix<T>::Matrix(size_t n, size_t m, V &box) : Matrix(n, m) {
    Iterator<T> it = this-begin();
    for (auto e : box) *it = *e, ++it;
}

template <typename T>
size_t Matrix<T>::rows() const noexcept { return this->m_rows; }
template <typename T>
size_t Matrix<T>::columns() const noexcept { return this->m_columns; }

template <typename T> size_t Matrix<T>::globalSize() const noexcept { return this->m_rows * this->m_columns; }

template <typename T>
Iterator<T> Matrix<T>::begin() const { return Iterator<T>(this->m_ptr, this->m_rows, this->m_columns); }
template <typename T>
Iterator<T> Matrix<T>::end() const {
    return Iterator<T>(this->m_ptr, this->m_rows, this->m_columns, this->globalSize()); }

//template <typename T>
//Iterator<T> Matrix<T>::cbegin() const {
//    const auto tmp = m_ptr;
//    return Iterator<T>(tmp, this->m_rows, this->m_columns);
//}
//template <typename T>
//Iterator<T> Matrix<T>::cend() const {
//    const auto tmp = m_ptr;
//    return Iterator<T>(tmp, this->m_rows, this->m_columns, this->globalSize()); }


template <typename T>
const T& Matrix<T>::get(size_t i, size_t j) const { return (*this)[i][j]; }
template <typename T>
T& Matrix<T>::set(size_t i, size_t j) { return (*this)[i][j]; }

template <typename T>
bool Matrix<T>::isSquare() const { return this->m_rows == this->m_columns; }

template <typename T>
bool Matrix<T>::isEqualSizes(const Matrix<T> &b) const noexcept {
    return (this->m_rows == b.m_rows && this->m_columns == b.m_columns); }

template <typename T>
bool Matrix<T>::isSuitForMult(const Matrix<T> &b) const noexcept { return (this->m_columns == b.m_rows); }

template <typename T>
Matrix<T>& Matrix<T>::additionToSelf(const T& term) {
    for (size_t i = 0; i < this->m_rows; i++)
        for (size_t j = 0; j < this->m_columns; j++) (*this)(i, j) += term;
    return (*this);
}
template <typename T>
Matrix<T>& Matrix<T>::additionToSelf(const Matrix<T>& mat) {
    { time_t _time = time(NULL);
    if (!this->isEqualSizes(mat))
        throw OperationException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time)); }
    for (size_t i = 0; i < this->m_rows; i++)
        for (size_t j = 0; j < this->m_columns; j++)
            (*this)(i, j) += mat(i, j);
    return (*this);
}
template <typename T>
Matrix<T> Matrix<T>::addition(const T& term) const {
    Matrix<T> tmp (*this); tmp.additionToSelf(term);
    return tmp;
}
template <typename T>
Matrix<T> Matrix<T>::addition(const Matrix<T>& mat) const {
    { time_t _time = time(NULL);
    if (!this->isEqualSizes(mat))
        throw OperationException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time)); }
    Matrix<T> tmp (*this); tmp.additionToSelf(mat);
    return tmp;
}

template <typename T>
Matrix<T>& Matrix<T>::multToSelf(const T& term) {
    for (size_t i = 0; i < this->m_rows; i++)
        for (size_t j = 0; j < this->m_columns; j++)
            (*this)(i, j) *= term;
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::multToSelf(const Matrix<T>& mat) {
    *this = move(this->multiplication(mat));
    return *this;
}
template <typename T>
Matrix<T> Matrix<T>::multiplication(const T& term) const {
    Matrix<T> tmp (*this); tmp.multToSelf(term);
    return tmp;
}
template <typename T>
Matrix<T> Matrix<T>::multiplication(const Matrix<T>& mat) const {
    {
        time_t _time = time(NULL);
        if (!this->isSuitForMult(mat))
            throw MultiplicationException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time));
    }
    Matrix<T> res(this->m_rows, mat.m_columns);
    for (size_t i = 0; i < this->m_rows; ++i) // n
        for (size_t j = 0; j < mat.m_columns; ++j) { // m
            res(i, j) = 0;
            for (size_t k = 0; k < this->m_columns; ++k) // l
                res(i, j) += (*this)(i, k) * mat(k, j); }
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::multiplicationVinograd(const Matrix<T>& mat) const {
    {
        time_t _time = time(NULL);
        if (!this->isSuitForMult(mat))
            throw MultiplicationException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time));
    }
    Matrix<T> res(this->m_rows, mat.m_columns);
    // матрицы this размером a x b на матрицу mat
    // размером b x c.Результат записывается в матрицу res размером a x c
    // вычисление rowFactors для G
 
    MatrixLine rowFactor(this->m_rows);
    for (size_t i = 0; i < this->m_rows; i++) {
        rowFactor[i] = 0;
        for (size_t k = 0; k < this->m_columns / 2; k++)
            rowFactor[i] = rowFactor[i] + (*this)[i][2 * k] * (*this)[i][2 * k + 1];
    }

    // вычисление	columnFactors для H
    MatrixLine columnFactor(mat.m_columns);
    for (size_t i = 0; i < mat.m_columns; i++) {
        columnFactor[i] = 0;
        for (size_t k = 0; k < this->m_columns / 2; k++)
            columnFactor[i] = columnFactor[i] + mat[2 * k][i] * mat[2 * k + 1][i];
    }

    // вычисление матрицы R
    for (size_t i = 0; i < this->m_rows; i++)
        for (size_t j = 0; j < mat.m_columns; j++) {
            res[i][j] = -rowFactor[i] - columnFactor[j];
            for (size_t k = 0; k < this->m_columns / 2; k++)
                res(i, j) += ((*this)(i, 2 * k) + mat[2 * k + 1][j])
                * ((*this)[i][2 * k + 1] + mat[2 * k][j]);
        }

    if ((this->m_columns % 2) == 1)
        for (size_t i = 0; i < this->m_rows; ++i)
            for (size_t j = 0; j < mat.m_columns; ++j)
                res[i][j] += (*this)[i][this->m_columns - 1] * mat[this->m_columns - 1][j];

    return res;
}

template <typename T>
Matrix<T> Matrix<T>::multiplicationVinogradOptimised(const Matrix<T>& mat) const {
    {   time_t _time = time(NULL);
        if (!this->isSuitForMult(mat))
            throw MultiplicationException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time)); }

    const size_t m = this->m_rows, q = mat.m_columns;
    size_t temp_size = (this->m_columns >> 1) << 1;
    Matrix<T> res(m, q);
    MatrixLine rowFactor(m), columnFactor(q);
    T temp;

    //calcRow(std::ref(rowFactor), m, temp_size, 0, m);
    //calcColumn(std::ref(columnFactor), std::ref(mat), q, temp_size, 0, q);

    for (size_t i = 0; i < m; ++i) {
        temp = 0;
        for (size_t k = 0; k < temp_size; k += 2)
            temp += ((*this)(i, k) * (*this)(i, k + 1));
        rowFactor[i] = temp;
    }
    for (size_t i = 0; i < q; ++i) {
        temp = 0;
        for (size_t k = 0; k < temp_size; k += 2)
            temp += mat(k, i) * mat(k + 1, i);
        columnFactor[i] = temp;
    }

    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < q; ++j) {
            temp = -(rowFactor[i] + columnFactor[j]);
            for (size_t k = 0; k < temp_size; k += 2) {
               temp += ((*this)(i, k) + mat(k + 1, j))
                    * ((*this)(i, k + 1) + mat(k, j));
            }
            res(i, j) = temp;
        }
    if ((temp_size) != this->m_columns)
        for (size_t i = 0, t = temp_size; i < m; ++i) 
            for (size_t j = 0; j < q; ++j)
                res(i, j) += (*this)(i, t) * mat(t, j);
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::division(const T& term) const {
    Matrix<T> tmp (*this); tmp.divToSelf(term);
    return tmp;
}
template <typename T>
Matrix<T>& Matrix<T>::divToSelf(const T& term) {
    { time_t _time = time(NULL);
    if (term == 0)
        throw ZeroDivException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time)); }
    for (size_t i = 0; i < this->m_rows; i++)
        for (size_t j = 0; j < this->m_columns; j++)
            (*this)(i, j) /= term;
    return *this;
}
template <typename T>
Matrix<T> Matrix<T>::operator/(const T& term) const { return this->division(term); }
template <typename T>
Matrix<T>& Matrix<T>::operator/=(const T& term) { return this->divToSelf(term); }

template <typename T>
Matrix<T>& Matrix<T>::subToSelf(const T& term) { return this->additionToSelf(-term); }
template <typename T>
Matrix<T> &Matrix<T>::subToSelf(const Matrix<T>& mat) {
    time_t _time = time(NULL);
    if (!this->isEqualSizes(mat))
        throw OperationException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time));
    for (size_t i = 0; i < this->m_rows; i++)
        for (size_t j = 0; j < this->m_columns; j++)
            (*this)(i, j) -= mat(i, j);
    return *this;
}
template <typename T>
Matrix<T> Matrix<T>::subtraction(const T& term) const { return this->addition(-term); }
template <typename T>
Matrix<T> Matrix<T>::subtraction(const Matrix<T>& mat) const {
    { time_t _time = time(NULL);
    if (!this->isEqualSizes(mat))
        throw OperationException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time)); }
    Matrix<T> tmp (*this); tmp.subToSelf(mat);
    return tmp;
}

template <typename T>
void Matrix<T>::inverseElements() {
    for (size_t i = 0; i < this->m_rows; i++)
        for (size_t j = 0; j < this->m_columns; j++)
            (*this)(i, j) = -(*this)(i, j);
}

template <typename T>
Matrix<T>& Matrix<T>::makeTransposed() {
    Matrix<T> tmp(this->m_columns, this->m_rows);
    for (size_t i = 0; i < this->m_rows; i++)
        for (size_t j = 0; j < this->m_columns; j++)
            tmp(j, i) = (*this)(i, j);
    *this = move(tmp);
    return *this;
}
template <typename T>
Matrix<T> Matrix<T>::transposed() const {
    Matrix<T> tmp(*this); tmp.makeTransposed();
    return tmp;
}

template <typename T>
Matrix<T> Matrix<T>::pow(int degree) const {
    time_t temp = time(NULL);
    if (degree < 0) throw PowException(__FILE__, typeid(*this).name(), __LINE__, ctime(&temp));
    if (!this->isSquare())  throw SqOnlyException(__FILE__, typeid(*this).name(), __LINE__, ctime(&temp));
    Matrix<T> result(this->m_rows, this->m_columns);
    result.makeUnitMatrix();
    for (int i = 0; i < degree; i++)
        result *= (*this);
    return result;
}

template <typename T>
void Matrix<T>::makeZero() {
    for (size_t i = 0; i < this->m_rows; i++)
        for (size_t j = 0; j < this->m_columns; j++)
            (*this)(i, j) = 0;
}

template <typename T>
void Matrix<T>::makeUnitMatrix() {
    time_t temp = time(NULL);
    if (!this->isSquare())  throw SqOnlyException(__FILE__, typeid(*this).name(), __LINE__, ctime(&temp));
    this->makeZero();
    for (size_t i = 0; i < this->m_rows; i++)
        (*this)[i][i] = 1;
}

template <typename T>
void Matrix<T>::makeCopyFrom(const Matrix<T> &copy) {
    for (size_t i = 0; i < this->m_rows; i++)
        for (size_t j = 0; j < this->m_columns; j++) (*this)[i][j] = copy[i][j];
}

template <typename T>
void Matrix<T>::makeCopyFrom(initializer_list<initializer_list<T> > &list) {
    time_t temp = time(NULL);
    for (size_t i = 0; i < this->m_rows; i++) {
        if (this->m_columns != list.begin()[i].size())
            throw UniformException(__FILE__, typeid(*this).name(), __LINE__, ctime(&temp));
        for (size_t j = 0; j < this->m_columns; j++)
            (*this)[i][j] = list.begin()[i].begin()[j];
    }
}

template <typename T>
void Matrix<T>::initSizes(size_t &n, size_t &m) { this->m_rows = n, this->m_columns = m; }
template <typename T>
void Matrix<T>::allocMatrix(size_t n, size_t m) { initSizes(n, m), this->alloc_ptr(); }

template <typename T>
inline void Matrix<T>::_move(Matrix<T>&& copy) {
    initSizes(copy.m_rows, copy.m_columns);
    this->m_ptr = copy.m_ptr;
    copy.m_ptr = nullptr, copy.m_rows = copy.m_columns = 0;
}


template <typename T>
void Matrix<T>::alloc_ptr() {
    MatrixLine* temp = new (std::nothrow) MatrixLine[m_rows];
    time_t _time = time(NULL);
    if (!temp)
        throw MemoryException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time));
    m_ptr = shared_ptr<MatrixLine[]> (temp);
    for (size_t i = 0; i < m_rows; i++) {
        m_ptr[i] = std::move(MatrixLine(m_columns));
    }
}
#endif // MATRIX_OPERATIONS_HPP
