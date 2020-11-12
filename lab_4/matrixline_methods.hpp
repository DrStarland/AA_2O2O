#ifndef MATRIXLINE_METHODS_HPP
#define MATRIXLINE_METHODS_HPP

#include "matrix.h"

template <typename T>
Matrix<T>::MatrixLine::MatrixLine(size_t len) : MatrixLine(len, nullptr) {}
template <typename T>
Matrix<T>::MatrixLine::MatrixLine(MatrixLine &&copy) { this->_move(std::move(copy)); }
template <typename T>
Matrix<T>::MatrixLine::MatrixLine(const MatrixLine &copy) : MatrixLine(copy.m_len, &copy) {}
template <typename T>
Matrix<T>::MatrixLine::MatrixLine(size_t n, const MatrixLine* source) {
    this->m_len = n, this->alloc_ptr();
    if (source != nullptr)
        for (size_t i = 0; i < this->m_len; i++)
            this->m_ptr[i] = source->m_ptr[i];
}

template <typename T>
void Matrix<T>::MatrixLine::alloc_ptr() {
    T* temp = new (std::nothrow) T[m_len];
    time_t _time = time(NULL);
    if (!temp)
        throw MemoryException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time));
    m_ptr = shared_ptr<T[]> (temp);
}

template <typename T>
typename Matrix<T>::MatrixLine& Matrix<T>::MatrixLine::operator=(MatrixLine&& copy) {
    this->_move(std::move(copy));
    return *this;
}

template <typename T>
T& Matrix<T>::MatrixLine::operator[](size_t ind) {
    time_t _time = time(NULL);
    if (this->m_len <= ind)
        throw IndexException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time));
    return this->m_ptr[ind];
}

template <typename T>
const T& Matrix<T>::MatrixLine::operator[](size_t ind) const {
    time_t _time = time(NULL);
    if (this->m_len <= ind)
        throw IndexException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time));
    return this->m_ptr[ind];
}

template <typename T>
void Matrix<T>::MatrixLine::_move(MatrixLine &&copy) {
    this->m_len = copy.m_len, this->m_ptr = copy.m_ptr;
    copy.m_ptr = nullptr, copy.m_len = 0;
}

#endif // MATRIXLINE_METHODS_HPP
