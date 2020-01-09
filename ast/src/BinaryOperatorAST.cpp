#include "../BinaryOperatorAST.h"
#include <stdexcept>

using namespace AST;

BinaryOperatorAST::BinaryOperatorAST(std::unique_ptr<IAST> && left, std::unique_ptr<IAST> && right, Type t)
        : mLeft(std::move(left))
        , mRight(std::move(right))
        , mType(t)
{
    if (mLeft->getResultType() != ValueType::Number)
    {
        throw std::logic_error("For operation " + typeToString(t) + " invalid left type" + valueTypeToString(mLeft->getResultType()));
    }

    if (mRight->getResultType() != ValueType::Number)
    {
        throw std::logic_error("For operation " + typeToString(t) + " invalid right type" + valueTypeToString(mRight->getResultType()));
    }

    mValueType = mLeft->getResultType();
}

void BinaryOperatorAST::accept(IASTVisitor & v) const
{
    v.visit(*this);
}

void BinaryOperatorAST::acceptLeft(IASTVisitor & v) const
{
    mLeft->accept(v);
}

void BinaryOperatorAST::acceptRight(IASTVisitor & v) const
{
    mRight->accept(v);
}

BinaryOperatorAST::Type BinaryOperatorAST::getType() const
{
    return mType;
}

ValueType BinaryOperatorAST::getResultType() const
{
    return mValueType;
}

std::string BinaryOperatorAST::typeToString(BinaryOperatorAST::Type t)
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
    std::ostream & operator << (std::ostream & out, BinaryOperatorAST::Type t)
    {
        out << BinaryOperatorAST::typeToString(t);
        return out;
    }
}