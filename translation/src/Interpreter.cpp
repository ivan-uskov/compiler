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
#include "ast/DoubleAST.h"

#include <stdexcept>

using namespace Translation;
using namespace AST;

namespace
{
    int calc(int left, int right, NumberBinaryOperatorAST::Type t)
    {
        switch (t)
        {
            case NumberBinaryOperatorAST::Type::Sum: return left + right;
            case NumberBinaryOperatorAST::Type::Sub: return left - right;
            case NumberBinaryOperatorAST::Type::Mul: return left * right;
            case NumberBinaryOperatorAST::Type::Div: return left / right;
            default: throw std::logic_error("invalid binary operator");
        }
    }

    template <typename T1, typename T2>
    double calc(T1 left, T2 right, NumberBinaryOperatorAST::Type t)
    {
        switch (t)
        {
            case NumberBinaryOperatorAST::Type::Sum: return left + right;
            case NumberBinaryOperatorAST::Type::Sub: return left - right;
            case NumberBinaryOperatorAST::Type::Mul: return left * right;
            case NumberBinaryOperatorAST::Type::Div: return left / right;
            default: throw std::logic_error("invalid binary operator");
        }
    }

    template <typename T1, typename T2>
    bool calcCompare(T1 left, T2 right, CompareBinaryOperatorAST::Type t)
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
    if (!isNumberType(right.type))
    {
        throw std::logic_error("invalid right operand for evaluating binary ast");
    }
    if (!isNumberType(left.type))
    {
        throw std::logic_error("invalid left operand for evaluating binary ast");
    }

    Var var{op.getResultType()};
    if (left.type == ValueType::Double && right.type == ValueType::Double)
    {
        var.doubleVal = calc(left.doubleVal, right.doubleVal, op.getType());
    }
    else if (left.type == ValueType::Double && right.type == ValueType::Int)
    {
        var.doubleVal = calc(left.doubleVal, right.intVal, op.getType());
    }
    else if (left.type == ValueType::Int && right.type == ValueType::Double)
    {
        var.doubleVal = calc(left.intVal, right.doubleVal, op.getType());
    }
    else
    {
        var.intVal = calc(left.intVal, right.intVal, op.getType());
    }

    mStack.push(var);
}

void Interpreter::visit(IntAST const& op)
{
    mStack.push(Var{AST::ValueType::Int, op.getValue()});
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
            mScope[op.getId()].intVal = value.intVal;
            break;
        case ValueType::String:
            mScope[op.getId()].strVal = value.strVal;
            break;
        case ValueType::Bool:
            mScope[op.getId()].boolVal = value.boolVal;
            break;
        case ValueType::Double:
            mScope[op.getId()].doubleVal = value.doubleVal;
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
            var.intVal = mScope[op.getId()].intVal;
            break;
        case ValueType::String:
            var.strVal = mScope[op.getId()].strVal;
            break;
        case ValueType::Bool:
            var.boolVal = mScope[op.getId()].boolVal;
            break;
        case ValueType::Double:
            var.doubleVal = mScope[op.getId()].doubleVal;
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
            mOut << arg.intVal;
            break;
        case ValueType::String:
            mOut << arg.strVal;
            break;
        case ValueType::Bool:
            mOut << (arg.boolVal ? "true" : "false");
            break;
        case ValueType::Double:
            mOut << arg.doubleVal;
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
        throw std::logic_error("invalid stack size for evaluating compare ast");
    }

    auto right = mStack.top();
    mStack.pop();
    auto left = mStack.top();
    mStack.pop();
    if (!isNumberType(right.type))
    {
        throw std::logic_error("invalid right operand for evaluating compare ast");
    }
    if (!isNumberType(left.type))
    {
        throw std::logic_error("invalid left operand for evaluating compare ast");
    }

    Var var{op.getResultType()};
    if (left.type == ValueType::Double && right.type == ValueType::Double)
    {
        var.boolVal = calcCompare(left.doubleVal, right.doubleVal, op.getType());
    }
    else if (left.type == ValueType::Double && right.type == ValueType::Int)
    {
        var.boolVal = calcCompare(left.doubleVal, right.intVal, op.getType());
    }
    else if (left.type == ValueType::Int && right.type == ValueType::Double)
    {
        var.boolVal = calcCompare(left.intVal, right.doubleVal, op.getType());
    }
    else
    {
        var.boolVal = calcCompare(left.intVal, right.intVal, op.getType());
    }
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

void Interpreter::visit(DoubleAST const &op)
{
    mStack.push(Var{AST::ValueType::Double, 0, false, "", op.getValue()});
}
