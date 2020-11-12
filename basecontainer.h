#ifndef BASECONTAINER_H
#define BASECONTAINER_H

#include <memory>

class BasicContainer
{
protected:
    size_t m_rows, m_columns;
public:
    BasicContainer() = default;
    virtual size_t globalSize() const noexcept = 0;
    virtual ~BasicContainer() = default;
};
#endif // BASECONTAINER_H
