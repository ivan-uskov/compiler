#include "ast/CompareBinaryOperatorAST.h"

#include <stdexcept>

using namespace AST;

CompareBinaryOperatorAST::CompareBinaryOperatorAST(std::unique_ptr<IAST> && left, std::unique_ptr<IAST> && right, Type t)
        : mLeft(std::move(left))
        , mRight(std::move(right))
        , mType(t)
{
    if (!isNumberType(mLeft->getResultType()))
    {
        throw std::logic_error("For operation " + typeToString(t) + " invalid left type " + valueTypeToString(mLeft->getResultType()));
    }

    if (!isNumberType(mRight->getResultType()))
    {
        throw std::logic_error("For operation " + typeToString(t) + " invalid right type " + valueTypeToString(mRight->getResultType()));
    }

    mValueType = ValueType::Bool;
}

void CompareBinaryOperatorAST::accept(IASTVisitor & v) const
{
    v.visit(*this);
}

void CompareBinaryOperatorAST::acceptLeft(IASTVisitor & v) const
{
    mLeft->accept(v);
}

void CompareBinaryOperatorAST::acceptRight(IASTVisitor & v) const
{
    mRight->accept(v);
}

CompareBinaryOperatorAST::Type CompareBinaryOperatorAST::getType() const
{
    return mType;
}

ValueType CompareBinaryOperatorAST::getResultType() const
{
    return mValueType;
}

std::string CompareBinaryOperatorAST::typeToString(CompareBinaryOperatorAST::Type t)
{
    switch (t)
    {
        case Type::Less:
            return "<";
        case Type::More:
            return ">";
        case Type::Equals:
            return "==";
        default:
            return "";
    }
}

ValueType CompareBinaryOperatorAST::getLeftType() const
{
    return mLeft->getResultType();
}

ValueType CompareBinaryOperatorAST::getRightType() const
{
    return mRight->getResultType();
}

namespace AST
{
    std::ostream & operator << (std::ostream & out, CompareBinaryOperatorAST::Type t)
    {
        out << CompareBinaryOperatorAST::typeToString(t);
        return out;
    }
}
