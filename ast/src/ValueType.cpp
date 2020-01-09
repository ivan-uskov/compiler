#include "ast/ValueType.h"

namespace AST
{
    std::ostream & operator << (std::ostream & out, ValueType const& t)
    {
        if (t == ValueType::Error)
        {
            out << "error";
        }
        if (t == ValueType::Void)
        {
            out << "void";
        }
        if (t == ValueType::Number)
        {
            out << "number";
        }
        if (t == ValueType::String)
        {
            out << "string";
        }
        return out;
    }
}