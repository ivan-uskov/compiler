#include "../BinaryOperatorAST.h"

BinaryOperatorAST::BinaryOperatorAST(std::unique_ptr<IAST> && left, std::unique_ptr<IAST> && right, Type t)
        : mLeft(std::move(left))
        , mRight(std::move(right))
        , mType(t)
{
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

std::ostream & operator << (std::ostream & out, BinaryOperatorAST::Type t)
{
    if (t == BinaryOperatorAST::Type::Sum)
    {
        out << "+";
    }
    else
    {
        out << "*";
    }
    return out;
}