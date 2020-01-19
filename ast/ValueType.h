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
        Bool = 4,
        Double = 5,

        IntArray = 6,
        StringArray = 7,
        BoolArray = 8,
        DoubleArray = 9
    };

    std::ostream & operator << (std::ostream & out, ValueType const& t);
    std::string valueTypeToString(ValueType const& t);
    bool isNumberType(ValueType const& t);
    bool isArrayType(ValueType const& t);
}