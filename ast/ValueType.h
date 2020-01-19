#pragma once

#include <ostream>

namespace AST
{
    enum ValueType
    {
        Error = 0,
        Void = 1,
        Int = 2,
        String = 3,
        Bool = 4
    };

    std::ostream & operator << (std::ostream & out, ValueType const& t);
    std::string valueTypeToString(ValueType const& t);
}