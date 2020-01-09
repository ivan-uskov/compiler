#pragma once

#include <ostream>

namespace AST
{
    enum ValueType
    {
        Error = 0,
        Void = 1,
        Number = 2,
        String = 3
    };

    std::ostream & operator << (std::ostream & out, ValueType const& t);
}