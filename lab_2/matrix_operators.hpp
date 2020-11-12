#ifndef MATRIX_OPERATORS_HPP
#define MATRIX_OPERATORS_HPP
#include "matrix.h"

template <typename T>
typename Matrix<T>::MatrixLine& Matrix<T>::operator[](size_t ind) {
    time_t temp = time(NULL);
    if (this->m_rows <= ind)
        throw IndexException(__FILE__, typeid(*this).name(), __LINE__, ctime(&temp));
    return this->m_ptr[ind];
}
template <typename T>
const typename Matrix<T>::MatrixLine& Matrix<T>::operator[](size_t ind) const {
    time_t temp = time(NULL);
    if (this->m_rows <= ind)
        throw IndexException(__FILE__, typeid(*this).name(), __LINE__, ctime(&temp));
    return this->m_ptr[ind];
}

template <typename T>
T& Matrix<T>::operator() (size_t i, size_t j) {
    time_t temp = time(NULL);
    if (this->m_rows <= i || this->m_columns <= j)
        throw IndexException(__FILE__, typeid(*this).name(), __LINE__, ctime(&temp));
    return this->m_ptr[i][j];
}
template <typename T>
const T& Matrix<T>::operator() (size_t i, size_t j) const {
    time_t temp = time(NULL);
    if (this->m_rows <= i || this->m_columns <= j)
        throw IndexException(__FILE__, typeid(*this).name(), __LINE__, ctime(&temp));
    return this->m_ptr[i][j];
}

template <typename T>
Matrix<T>::operator bool() const noexcept { return this->m_rows && this->m_columns; }

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &b) const {
    bool verdict = false;
    if (this->isEqualSizes(b)) {
        verdict = true;
        for (size_t i = 0; i < this->m_rows; i++)
            for (size_t j = 0; j < this->m_columns; j++)
                if ((*this)(i, j) != b(i, j))
                    verdict = false, i = this->m_rows, j = this->m_columns; // завершение циклов
    } return verdict;
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix<T> &b) const { return !(*this == b); }

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& copy) {
    if ((*this) != copy) {
        if (!isEqualSizes(*this, copy))
            allocMatrix(copy.m_rows, copy.m_columns);
        this->makeCopyFrom(copy); }
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& copy) {
    this->_move(std::move(copy));
    return *this;
}
template <typename T>
Matrix<T>& Matrix<T>::operator=(initializer_list<initializer_list<T>> list) {
    allocMatrix(list.size(), list.begin()[0].size()), this->makeCopyFrom(list);
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& mat) { return this->additionToSelf(mat); }

template <typename T>
Matrix<T> Matrix<T>::operator-() const {
    Matrix<T> tmp(*this); tmp.inverseElements();
    return tmp;
}
template <typename T>
Matrix<T> Matrix<T>::operator+() const { return Matrix<T>(*this); }

template <typename T>
Matrix<T> Matrix<T>::operator+(const T& term) const { return this->addition(term); }
template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& mat) const { return this->addition(mat); }
template <typename T>
Matrix<T>& Matrix<T>::operator+=(const T& term) { return this->additionToSelf(term); }

template <typename T>
Matrix<T> Matrix<T>::operator-(const T& term) const { return this->subtraction(term); }
template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& mat) const { return this->subtraction(mat); }
template <typename T>
Matrix<T>& Matrix<T>::operator-=(const T& term) { return this->subToSelf(term); }
template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& mat) { return this->subToSelf(mat); }

template <typename T>
Matrix<T> Matrix<T>::operator*(const T& term) const { return this->multiplication(term); }
template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& mat) const { return this->multiplication(mat); }
template <typename T>
Matrix<T>& Matrix<T>::operator*=(const T& term) { return this->multToSelf(term); }
template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& mat) { return this->multToSelf(mat); }

template <typename T>
std::ostream& operator<< (std::ostream &out, const Matrix<T> &matr) {
    for (size_t i = 0; i < matr.m_rows; i++) {
        for (size_t j = 0; j < matr.m_columns; j++)
            out << matr[i][j] << " ";
        out << endl;
     } return out;
}
template <typename T>
std::istream& operator>> (std::istream &in, Matrix<T> &matr) {
    for (size_t i = 0; i < matr.m_rows; i++)
        for (size_t j = 0; j < matr.m_columns; j++)
            in >> matr[i][j];
    return in;
}
#endif // MATRIX_OPERATORS_HPP
