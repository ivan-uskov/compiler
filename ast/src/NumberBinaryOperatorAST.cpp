#include "ast/NumberBinaryOperatorAST.h"
#include <stdexcept>

using namespace AST;

NumberBinaryOperatorAST::NumberBinaryOperatorAST(std::unique_ptr<IAST> && left, std::unique_ptr<IAST> && right, Type t)
        : mLeft(std::move(left))
        , mRight(std::move(right))
        , mType(t)
{
    if (mLeft->getResultType() != ValueType::Int)
    {
        throw std::logic_error("For operation " + typeToString(t) + " invalid left type" + valueTypeToString(mLeft->getResultType()));
    }

    if (mRight->getResultType() != ValueType::Int)
    {
        throw std::logic_error("For operation " + typeToString(t) + " invalid right type" + valueTypeToString(mRight->getResultType()));
    }

    mValueType = mLeft->getResultType();
}

void NumberBinaryOperatorAST::accept(IASTVisitor & v) const
{
    v.visit(*this);
}

void NumberBinaryOperatorAST::acceptLeft(IASTVisitor & v) const
{
    mLeft->accept(v);
}

void NumberBinaryOperatorAST::acceptRight(IASTVisitor & v) const
{
    mRight->accept(v);
}

NumberBinaryOperatorAST::Type NumberBinaryOperatorAST::getType() const
{
    return mType;
}

ValueType NumberBinaryOperatorAST::getResultType() const
{
    return mValueType;
}

std::string NumberBinaryOperatorAST::typeToString(NumberBinaryOperatorAST::Type t)
{
    switch (t)
    {
        case Type::Sum:
            return "+";
        case Type::Sub:
            return "-";
        case Type::Mul:
            return "*";
        case Type::Div:
            return "/";
        default:
            return "";
    }
}

namespace AST
{
    std::ostream & operator << (std::ostream & out, NumberBinaryOperatorAST::Type t)
    {
        out << NumberBinaryOperatorAST::typeToString(t);
        return out;
    }
}