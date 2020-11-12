#ifndef ITERATOR_METHODS_HPP
#define ITERATOR_METHODS_HPP

#include "owniterator.h"

template <typename T>
Iterator<T>& Iterator<T>::operator=(const Iterator<T>& copy) {
    arr = copy.arr, m_num = copy.m_num, index = copy.index;
    return *this;
}

template <typename T>
bool Iterator<T>::operator!=(Iterator const& other) const noexcept { return index != other.index; }
template <typename T>
bool Iterator<T>::operator==(Iterator const& other) const noexcept { return index == other.index; }
template <typename T>
Iterator<T>::operator bool() const {
    return !arr.expired();
}
template <typename T>
T& Iterator<T>::operator*() {
    time_t _time = time(NULL);
    if (arr.expired()) //  ?
        throw DelObjException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time));
    if (index >= m_num)
        throw IteratorException(__FILE__, typeid(*this).name(), __LINE__, ctime(&_time));

    shared_ptr<typename Matrix<T>::MatrixLine[]> a(arr);
    return a[index / m_col][index % m_col];        // i = index / m_col, j = index % m_col;
}
template <typename T>
Iterator<T>& Iterator<T>::operator++() noexcept {
    if (index < m_num)
        index++;
    return *this;
}
template <typename T>
Iterator<T> Iterator<T>::operator++(int) noexcept {
    Iterator<T> it(*this);
    ++(*this);
    return it;
}

#endif // ITERATOR_METHODS_HPP
