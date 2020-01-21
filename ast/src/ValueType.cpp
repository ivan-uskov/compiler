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

    bool isTypesMatch(ValueType const& varT, ValueType const& valT)
    {
        if (isNumberType(varT) && isNumberType(valT))
        {
            return true;
        }

        return varT == valT;
    }

    ValueType getArrayItemType(ValueType const& t)
    {
        switch (t)
        {
            case ValueType::DoubleArray:
                return ValueType::Double;
            case ValueType::IntArray:
                return ValueType::Int;
            case ValueType::StringArray:
                return ValueType::String;
            case ValueType::BoolArray:
                return ValueType::Bool;
            default:
                throw std::logic_error(valueTypeToString(t) + " is not a array type");
        }
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
                return "IntArray";
            case ValueType::BoolArray:
                return "BoolArray";
            case ValueType::StringArray:
                return "StringArray";
            case ValueType::DoubleArray:
                return "DoubleArray";
            default:
                return "";
        }
    }
}