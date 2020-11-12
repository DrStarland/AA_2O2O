#ifndef EXCEPTIONBASE_H
#define EXCEPTIONBASE_H

#include <exception>
#include <string>

using namespace std;
// переименовать
class MatrixException : public std::exception
{
protected:
    string errorInfo = "";
public:
    MatrixException(string fileName, string className, int line,
                   string time, string info);
    virtual const char *what() const noexcept override {
        return errorInfo.c_str();
    }
    virtual ~MatrixException() = default;
};

MatrixException::MatrixException(string fileName, string className,
                               int line, string time, string info)
{
    time.at(time.size() - 1) = ',';
    char temp[16];
    snprintf(temp, 12, ", line %d: ", line);
    errorInfo += (string(time) + " " + fileName + ", " + className + string(temp) +  info);
}

#endif // EXCEPTIONBASE_H
