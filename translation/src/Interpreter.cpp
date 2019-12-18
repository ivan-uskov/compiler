#include "../Interpreter.h"
#include "ast/NumberAST.h"
#include "ast/BinaryOperatorAST.h"
#include "ast/ExpressionPairAST.h"

#include <stdexcept>

using namespace Translation;
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

void Interpreter::visit(ExpressionPairAST const &op)
{
    op.acceptLeft(*this);
    op.acceptRight(*this);
}

std::vector<float> Interpreter::getValues() const
{
    auto stack = mStack;
    std::vector<float> res;
    res.reserve(stack.size());
    while (!stack.empty())
    {
        res.push_back(stack.top());
        stack.pop();
    }
    std::reverse(res.begin(), res.end());
    return res;
}
