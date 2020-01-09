#include "../Interpreter.h"
#include "ast/NumberAST.h"
#include "ast/BinaryOperatorAST.h"
#include "ast/ExpressionPairAST.h"
#include "ast/VariableDeclarationAST.h"
#include "ast/VariableAccessAST.h"
#include "ast/FunctionCallAST.h"
#include "ast/AssignmentAST.h"

#include <stdexcept>

using namespace Translation;
using namespace AST;

namespace
{
    double calc(double left, double right, BinaryOperatorAST::Type t)
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

void Interpreter::visit(VariableDeclarationAST const &op)
{
    mScope[op.getId()] = Var{op.getResultType()};
}

void Interpreter::visit(AssignmentAST const &op)
{
    op.acceptValue(*this);

    if (mStack.empty())
    {
        throw std::logic_error("invalid stack size for evaluating assignment");
    }

    auto value = mStack.top();
    mStack.pop();

    mScope[op.getId()].numVal = value; //TODO: add string support
}

void Interpreter::visit(VariableAccessAST const &op)
{
    mStack.push(mScope[op.getId()].numVal); //TODO: add string support
}

void Interpreter::visit(FunctionCallAST const &op)
{
    op.acceptArgument(*this);

    if (mStack.empty())
    {
        throw std::logic_error("invalid stack size for evaluating assignment");
    }

    auto arg = mStack.top();
    mStack.pop();

    mOut << arg;
}

Interpreter::Interpreter(std::ostream &out)
    : mOut(out)
{
}
