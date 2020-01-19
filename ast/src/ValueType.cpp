#include "ast/ValueType.h"

namespace AST
{
    std::ostream & operator << (std::ostream & out, ValueType const& t)
    {
        out << valueTypeToString(t);

        return out;
    }

    bool isNumberType(ValueType const& t)
    {
        return t == ValueType::Int || t == ValueType::Double;
    }

    bool isArrayType(ValueType const& t)
    {
        return t == ValueType::IntArray || t == ValueType::DoubleArray || t == ValueType::BoolArray || t == ValueType::StringArray;
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
            case ValueType::Double:
                return "double";
            case ValueType::IntArray:
                return "intArray";
            case ValueType::BoolArray:
                return "boolArray";
            case ValueType::StringArray:
                return "stringArray";
            case ValueType::DoubleArray:
                return "doubleArray";
            default:
                return "";
        }
    }
}