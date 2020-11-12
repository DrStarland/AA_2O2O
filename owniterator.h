#ifndef OWNITERATOR_H
#define OWNITERATOR_H

#include <iterator>
#include <memory>
#include "exceptions.h"

template  <typename T>
class Matrix;

using namespace std;

template <typename T>
class Iterator : public std::iterator<std::input_iterator_tag, T>
{
    friend class Matrix<T>;
private:
    Iterator(const shared_ptr<typename Matrix<T>::MatrixLine[]>&  a, size_t rows, size_t columns, size_t ind = 0) :
                arr(a), m_num(rows * columns), m_col(columns), index(ind) {}
    weak_ptr<typename Matrix<T>::MatrixLine[]> arr;
    // спросить про сокращение записи, 6-7
    size_t m_num, m_col, index;
public:
    Iterator(const Iterator &it) = default;
    ~Iterator() = default;
    bool operator!=(Iterator const& other) const noexcept;
    bool operator==(Iterator const& other) const noexcept;
    explicit operator bool() const;
    Iterator<T>& operator=(const Iterator<T>& copy);
    T& operator*();
    const T& operator*() const;
    // стрелочку
    Iterator<T>& operator++() noexcept;
    Iterator<T> operator++(int) noexcept;
};

#include "owniterator_methods.hpp"

#endif // OWNITERATOR_H
