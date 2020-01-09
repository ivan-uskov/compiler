#include "../BinaryOperatorAST.h"
#include <stdexcept>

using namespace AST;

BinaryOperatorAST::BinaryOperatorAST(std::unique_ptr<IAST> && left, std::unique_ptr<IAST> && right, Type t)
        : mLeft(std::move(left))
        , mRight(std::move(right))
        , mType(t)
{
    if (mLeft->getResultType() != mRight->getResultType())
    {
        throw std::logic_error("Invalid operand types");
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

namespace AST
{
    std::ostream & operator << (std::ostream & out, BinaryOperatorAST::Type t)
    {
        if (t == BinaryOperatorAST::Type::Sum)
        {
            out << "+";
        }
        else if (t == BinaryOperatorAST::Type::Mul)
        {
            out << "*";
        }
        else if (t == BinaryOperatorAST::Type::Sub)
        {
            out << "-";
        }
        else if (t == BinaryOperatorAST::Type::Div)
        {
            out << "/";
        }
        return out;
    }
}