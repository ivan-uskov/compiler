#include "../BoolBinaryOperatorAST.h"

#include <stdexcept>

using namespace AST;

BoolBinaryOperatorAST::BoolBinaryOperatorAST(std::unique_ptr<IAST> && left, std::unique_ptr<IAST> && right, Type t)
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

    mValueType = ValueType::Bool;
}

void BoolBinaryOperatorAST::accept(IASTVisitor & v) const
{
    v.visit(*this);
}

void BoolBinaryOperatorAST::acceptLeft(IASTVisitor & v) const
{
    mLeft->accept(v);
}

void BoolBinaryOperatorAST::acceptRight(IASTVisitor & v) const
{
    mRight->accept(v);
}

BoolBinaryOperatorAST::Type BoolBinaryOperatorAST::getType() const
{
    return mType;
}

ValueType BoolBinaryOperatorAST::getResultType() const
{
    return mValueType;
}

std::string BoolBinaryOperatorAST::typeToString(BoolBinaryOperatorAST::Type t)
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

namespace AST
{
    std::ostream & operator << (std::ostream & out, BoolBinaryOperatorAST::Type t)
    {
        out << BoolBinaryOperatorAST::typeToString(t);
        return out;
    }
}
