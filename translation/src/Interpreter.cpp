#include "../Interpreter.h"
#include "ast/IntAST.h"
#include "ast/NumberBinaryOperatorAST.h"
#include "ast/ExpressionPairAST.h"
#include "ast/VariableDeclarationAST.h"
#include "ast/VariableAccessAST.h"
#include "ast/FunctionCallAST.h"
#include "ast/AssignmentAST.h"
#include "ast/CompareBinaryOperatorAST.h"
#include "ast/IfAST.h"
#include "ast/WhileAST.h"
#include "ast/StringAST.h"

#include <stdexcept>

using namespace Translation;
using namespace AST;

namespace
{
    double calc(double left, double right, NumberBinaryOperatorAST::Type t)
    {
        if (t == NumberBinaryOperatorAST::Type::Sum)
        {
            return left + right;
        }
        else if (t == NumberBinaryOperatorAST::Type::Sub)
        {
            return left - right;
        }
        else if (t == NumberBinaryOperatorAST::Type::Mul)
        {
            return left * right;
        }
        else if (t == NumberBinaryOperatorAST::Type::Div)
        {
            return left / right;
        }

        throw std::logic_error("invalid binary operator");
    }

    bool calcCompare(double left, double right, CompareBinaryOperatorAST::Type t)
    {
        if (t == CompareBinaryOperatorAST::Type::Equals)
        {
            return left == right;
        }
        else if (t == CompareBinaryOperatorAST::Type::Less)
        {
            return left < right;
        }
        else if (t == CompareBinaryOperatorAST::Type::More)
        {
            return left > right;
        }

        throw std::logic_error("invalid compare binary operator");
    }
}

void Interpreter::visit(NumberBinaryOperatorAST const& op)
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
    if (right.type != AST::ValueType::Int)
    {
        throw std::logic_error("invalid right operand for evaluating binary ast");
    }
    if (left.type != AST::ValueType::Int)
    {
        throw std::logic_error("invalid left operand for evaluating binary ast");
    }

    left.numVal = calc(left.numVal, right.numVal, op.getType());
    mStack.push(left);
}

void Interpreter::visit(IntAST const& op)
{
    mStack.push(Var{AST::ValueType::Int, double(op.getValue())});
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

    if (mScope[op.getId()].type != value.type)
    {
        throw std::logic_error("assignment type missmatch");
    }

    switch (value.type)
    {
        case ValueType::Int:
            mScope[op.getId()].numVal = value.numVal;
            break;
        case ValueType::String:
            mScope[op.getId()].strVal = value.strVal;
            break;
        case ValueType::Bool:
            mScope[op.getId()].boolVal = value.boolVal;
            break;
        default:
            throw std::logic_error("unassignable type error");
    }
}

void Interpreter::visit(VariableAccessAST const &op)
{
    Var var{op.getResultType()};
    switch (var.type)
    {
        case ValueType::Int:
            var.numVal = mScope[op.getId()].numVal;
            break;
        case ValueType::String:
            var.strVal = mScope[op.getId()].strVal;
            break;
        case ValueType::Bool:
            var.boolVal = mScope[op.getId()].boolVal;
            break;
        default:
            throw std::logic_error("invalid var type error");
    }
    mStack.push(var);
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

    switch (arg.type)
    {
        case ValueType::Int:
            mOut << arg.numVal;
            break;
        case ValueType::String:
            mOut << arg.strVal;
            break;
        case ValueType::Bool:
            mOut << (arg.boolVal ? "true" : "false");
            break;
        default:
            throw std::logic_error("invalid var type error");
    }

    if (op.getName() == FunctionCallAST::PRINTLN)
    {
        mOut << std::endl;
    }
}

Interpreter::Interpreter(std::istream & in, std::ostream &out)
    : mIn(in)
    , mOut(out)
{
}

void Interpreter::visit(CompareBinaryOperatorAST const &op)
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
    if (right.type != AST::ValueType::Int)
    {
        throw std::logic_error("invalid right operand for evaluating binary ast");
    }
    if (left.type != AST::ValueType::Int)
    {
        throw std::logic_error("invalid left operand for evaluating binary ast");
    }

    Var var{op.getResultType()};
    var.boolVal = calcCompare(left.numVal, right.numVal, op.getType());
    mStack.push(var);
}

void Interpreter::visit(IfAST const &op)
{
    op.acceptCond(*this);

    if (mStack.empty())
    {
        throw std::logic_error("invalid stack size for evaluating binary ast");
    }

    auto cond = mStack.top();
    mStack.pop();

    if (cond.type != AST::ValueType::Bool)
    {
        throw std::logic_error("invalid cond type for evaluating if ast");
    }

    if (cond.boolVal)
    {
        op.acceptStmt(*this);
    }
}

void Interpreter::visit(WhileAST const &op)
{
    while (true)
    {
        op.acceptCond(*this);

        if (mStack.empty())
        {
            throw std::logic_error("invalid stack size for evaluating binary ast");
        }

        auto cond = mStack.top();
        mStack.pop();

        if (cond.type != AST::ValueType::Bool)
        {
            throw std::logic_error("invalid cond type for evaluating if ast");
        }

        if (cond.boolVal)
        {
            op.acceptStmt(*this);
        }
        else
        {
            break;
        }
    }
}

void Interpreter::visit(StringAST const &op)
{
    mStack.push(Var{AST::ValueType::String, 0, false, op.getValue()});
}