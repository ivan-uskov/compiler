#include "../Interpreter.h"
#include "ast/NumberAST.h"
#include "ast/BinaryOperatorAST.h"

#include <stdexcept>

using namespace AST;

namespace
{
    float calc(float left, float right, BinaryOperatorAST::Type t)
    {
        if (t == BinaryOperatorAST::Type::Sum)
        {
            return left + right;
        }
        else if (t == BinaryOperatorAST::Type::Sub)
        {
            return left - right;
        }
        else if (t == BinaryOperatorAST::Type::Mul)
        {
            return left * right;
        }
        else if (t == BinaryOperatorAST::Type::Div)
        {
            return left / right;
        }

        throw std::logic_error("invalid binary operator");
    }
}

void Interpreter::visit(BinaryOperatorAST const& op)
{
    op.acceptLeft(*this);
    op.acceptRight(*this);

    if (mStack.size() < 2)
    {
        throw std::logic_error("invalid stack size for evaluating binary ast");
    }

    auto right = mStack.top();
    mStack.pop();
    auto left = mStack.top();
    mStack.pop();
    mStack.push(calc(left, right, op.getType()));
}

void Interpreter::visit(NumberAST const& op)
{
    mStack.push(op.getValue());
}

float Interpreter::getValue() const
{
    if (mStack.size() != 1)
    {
        throw std::logic_error("value not calculated");
    }

    return mStack.top();
}
