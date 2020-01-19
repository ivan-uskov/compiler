#include "ast/ValueType.h"

namespace AST
{
    std::ostream & operator << (std::ostream & out, ValueType const& t)
    {
        out << valueTypeToString(t);

        return out;
    }

    std::string valueTypeToString(ValueType const& t)
    {
        switch (t)
        {
            case ValueType::Error:
                return "error";
            case ValueType::Void:
                return "void";
            case ValueType::Int:
                return "int";
            case ValueType::String:
                return "string";
            case ValueType::Bool:
                return "bool";
            default:
                return "";
        }
    }
}