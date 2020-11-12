#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "matrixexception.h"
#include <time.h>

class IndexException : public MatrixException
{
public:
    IndexException(string fileName, string className, int line,
                   const char *time, string info = "The index was greater than the permissible value.") :
        MatrixException(fileName, className, line, time, info) {};
    virtual ~IndexException() = default;
    virtual const char *what() const noexcept { return errorInfo.c_str(); }
};

class UniformException : public MatrixException
{
public:
    UniformException(string fileName, string className, int line,
                   const char *time, string info = "Invalid list sizes.") :
        MatrixException(fileName, className, line, time, info) {};
    virtual ~UniformException() = default;
    virtual const char *what() const noexcept { return errorInfo.c_str(); }
};

class PointerInitException : public MatrixException
{
public:
    PointerInitException(string fileName, string className, int line,
                   const char *time, string info = "The source is empty.") :
        MatrixException(fileName, className, line, time, info) {};
    virtual ~PointerInitException() = default;
    virtual const char *what() const noexcept { return errorInfo.c_str(); }
};

class MemoryException : public MatrixException
{
public:
    MemoryException(string fileName, string className, int line,
                   const char *time, string info = "Memory manager returned bad_alloc exception.") :
        MatrixException(fileName, className, line, time, info) {};
    virtual ~MemoryException() = default;
    virtual const char *what() const noexcept { return errorInfo.c_str(); }
};

class MultiplicationException : public MatrixException
{
public:
    MultiplicationException(string fileName, string className, int line,
                   const char *time, string info = "Invalid matrix multiplication attempt.") :
        MatrixException(fileName, className, line, time, info) {};
    virtual ~MultiplicationException() = default;
    virtual const char *what() const noexcept { return errorInfo.c_str(); }
};

class OperationException : public MatrixException
{
public:
    OperationException(string fileName, string className, int line,
                   const char *time, string info = "Different matrix sizes.") :
        MatrixException(fileName, className, line, time, info) {};
    virtual ~OperationException() = default;
    virtual const char *what() const noexcept { return errorInfo.c_str(); }
};

class PowException : public MatrixException
{
public:
    PowException(string fileName, string className, int line,
                   const char *time, string info = "Negative degree.") :
        MatrixException(fileName, className, line, time, info) {};
    virtual ~PowException() = default;
    virtual const char *what() const noexcept { return errorInfo.c_str(); }
};

class ZeroDivException : public MatrixException
{
public:
    ZeroDivException(string fileName, string className, int line,
                   const char *time, string info = "Here was spotted a zero division.") :
        MatrixException(fileName, className, line, time, info) {};
    virtual ~ZeroDivException() = default;
    virtual const char *what() const noexcept { return errorInfo.c_str(); }
};

class DelObjException : public MatrixException
{
public:
    DelObjException(string fileName, string className, int line,
                   const char *time, string info = "Attempt to access a remote object.") :
        MatrixException(fileName, className, line, time, info) {};
    virtual ~DelObjException() = default;
    virtual const char *what() const noexcept { return errorInfo.c_str(); }
};

class IteratorException : public MatrixException
{
public:
    IteratorException(string fileName, string className, int line,
                   const char *time, string info = "Inaccessible memory access attempt.") :
        MatrixException(fileName, className, line, time, info) {};
    virtual ~IteratorException() = default;
    virtual const char *what() const noexcept { return errorInfo.c_str(); }
};

class SqOnlyException : public MatrixException
{
public:
    SqOnlyException(string fileName, string className, int line,
                   const char *time, string info = "This operation can only be applied to square matrices") :
        MatrixException(fileName, className, line, time, info) {};
    virtual ~SqOnlyException() = default;
    virtual const char *what() const noexcept { return errorInfo.c_str(); }
};

#endif // EXCEPTIONS_H
