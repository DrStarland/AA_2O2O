#ifndef Matrix_H
#define Matrix_H

#include <memory>
#include <initializer_list>
#include <iostream>
#include "basecontainer.h"
#include "owniterator.h"
#include "exceptions.h"

template  <typename T>
class Matrix: public BasicContainer
{   // сделать внутренний класс
public:
    class MatrixLine; // описание вынесено за пределы описания этого класса, чтобы было проще просматривать
public:
    // конструкторы
    explicit Matrix(size_t N = 0); // конструктор по 1 размеру + базовый
    Matrix(size_t n, size_t m); // конструктор по двум размерам
    Matrix(initializer_list< initializer_list<T> > list); // конструктор через списки списков
    Matrix(size_t n, size_t m, T** ptr);     // конструктор из указателей добавить
    template <typename V> Matrix(size_t n, size_t m, V &box); // конструктор с произвольным контейнером
    // копирующие конструкторы
    explicit Matrix(const Matrix<T> &copy);
    Matrix(Matrix<T> &&copy) noexcept;
    // деструкторы
    virtual ~Matrix() {};
    // копирующе-изменяющие обычные операторы
    Matrix<T>& operator=(const Matrix<T>& copy);
    Matrix<T>& operator=(Matrix<T>&& copy);
    Matrix<T>& operator=(initializer_list<initializer_list<T> > list);
    // технические операторы
    T& operator() (size_t i, size_t j);
    const T& operator() (size_t i, size_t j) const;
    MatrixLine& operator[](size_t ind);
    const MatrixLine& operator[](size_t ind) const;
    const T& get(size_t i, size_t j) const; // сделать геттеры
    T& set(size_t i, size_t j);
    // унарные операторы
    Matrix<T> operator+() const;
    Matrix<T> operator-() const;
    void inverseElements(); // возврат
    // операторы обычных действий
        // сложение
    Matrix<T> addition(const Matrix<T>& mat) const;
    Matrix<T> addition(const T& term) const;
    Matrix<T>& additionToSelf(const Matrix<T>& mat); // вернуть ссылку
    Matrix<T>& additionToSelf(const T& term);
    Matrix<T> operator+(const Matrix<T>& mat) const;
    Matrix<T> operator+(const T& term) const;
    Matrix<T>& operator+=(const Matrix<T>& mat); // возвращать ссылку
    Matrix<T>& operator+=(const T& term);
        // вычитание
    Matrix<T> subtraction(const Matrix<T>& mat) const;
    Matrix<T> subtraction(const T& term) const;
    Matrix<T>& subToSelf(const Matrix<T>& mat);
    Matrix<T>& subToSelf(const T& term);
    Matrix<T> operator-(const Matrix<T>& mat) const;
    Matrix<T> operator-(const T& term) const;
    Matrix<T>& operator-=(const Matrix<T>& mat);
    Matrix<T>& operator-=(const T& term);
        // умножение
    Matrix<T> multiplication(const Matrix<T>& mat) const;
    Matrix<T> multiplication(const T& term) const;
    Matrix<T>& multToSelf(const Matrix<T>& mat);
    Matrix<T>& multToSelf(const T& term);
    Matrix<T> operator*(const Matrix<T>& mat) const;
    Matrix<T> operator*(const T& term) const;
    Matrix<T>& operator*=(const Matrix<T>& mat);
    Matrix<T>& operator*=(const T& term);
    Matrix<T> pow(int degree) const;
        // добавить деление на число
    Matrix<T> division(const T& term) const;
    Matrix<T>& divToSelf(const T& term);
    Matrix<T> operator/(const T& term) const;
    Matrix<T>& operator/=(const T& term);
    // логические операторы
    bool operator== (const Matrix<T> &b) const;
    bool operator!= (const Matrix<T> &b) const;
    explicit operator bool() const noexcept; // матрица нулевого размера - ложь
    // операторы ввода-вывода
    template <typename V> friend std::istream& operator>> (std::istream &in, Matrix<V> &matr);
    template <typename V> friend std::ostream& operator<< (std::ostream &out, const Matrix<V> &matr);
    // матричные операции
    Matrix<T> transposed() const;
    Matrix<T>& makeTransposed(); // ссылку
    Matrix<T>& operator~() const;
    T determinant() const; T det() const; // идентичные, просто с разным названием
    Matrix<T>& makeReversed();
    Matrix<T> reversed() const;
    Matrix<T> operator!();
    // матрично-числовые операции
    void makeZero(); // ~, !
    void makeUnitMatrix();
    bool isSquare() const;
    // итераторы
    Iterator<T> begin() const;
    Iterator<T> end() const;
//    Iterator<const T> begin() const;
//    Iterator<const T> end() const;
//    Iterator<const T> cbegin() const;     // константные итераторы
//    Iterator<const T> cend() const;    // для неконст матрицы cbegin, cend
    // различные мелкие функции и сравнения
    virtual size_t globalSize() const noexcept;
    bool isEqualSizes(const Matrix<T> &b) const noexcept;
    bool isSuitForMult(const Matrix<T> &b) const noexcept;
private:
    shared_ptr<MatrixLine[]> m_ptr;
    void alloc_ptr();
    Matrix(size_t n, size_t m, const Matrix<T> *source, unsigned char mode); // настоящий конструктор
    void makeCopyFrom(const Matrix<T> &copy);
    void makeCopyFrom(initializer_list<initializer_list<T> > &list);
    void initSizes(size_t &n, size_t &m);
    void allocMatrix(size_t n, size_t m);
    inline void _move(Matrix<T>&& copy);
};

template <typename T>
class Matrix<T>::MatrixLine
{
public:
    MatrixLine(size_t len = 1);
    virtual ~MatrixLine() = default;
    MatrixLine(const MatrixLine &copy);
    MatrixLine(MatrixLine &&copy);
    MatrixLine& operator=(MatrixLine&& copy);
    T& operator[](size_t index);
    const T& operator[](size_t ind) const;
private:
    MatrixLine(size_t n, const MatrixLine* source);
    void _move(MatrixLine &&copy);
    void alloc_ptr();
    shared_ptr<T[]> m_ptr;
    size_t m_len;
};

#include "matrixline_methods.hpp"
#include "matrix_operations.hpp"
#include "matrix_operators.hpp"

#endif // Matrix_H
