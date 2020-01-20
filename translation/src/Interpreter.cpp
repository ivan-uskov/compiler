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
#include "ast/ArrayAssignmentAST.h"
#include "ast/ArrayAccessAST.h"
#include "ast/BoolAST.h"

#include <stdexcept>
#include <sstream>
#include <algorithm>

using namespace Translation;
using namespace AST;

namespace
{
    template <typename T>
    std::string arrayToString(std::vector<T> const& arr)
    {
        std::ostringstream ostrm;
        for (size_t i = 0; i < arr.size(); ++i)
        {
            ostrm << arr[i];
            if (i < arr.size() - 1)
            {
                ostrm << ", ";
            }
        }
        return "{" + ostrm.str() + "}";
    }

    std::string arrayToString(std::vector<bool> const& arr)
    {
        std::vector<std::string> prepared;
        prepared.reserve(arr.size());
        for (auto v : arr)
        {
            prepared.emplace_back(v ? "true" : "false");
        }
        return arrayToString(prepared);
    }

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
        case ValueType::IntArray:
            mScope[op.getId()].intArray = value.intArray;
            break;
        case ValueType::BoolArray:
            mScope[op.getId()].boolArray = value.boolArray;
            break;
        case ValueType::StringArray:
            mScope[op.getId()].stringArray = value.stringArray;
            break;
        case ValueType::DoubleArray:
            mScope[op.getId()].doubleArray = value.doubleArray;
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
        case ValueType::IntArray:
            var.intArray = mScope[op.getId()].intArray;
            break;
        case ValueType::BoolArray:
            var.boolArray = mScope[op.getId()].boolArray;
            break;
        case ValueType::StringArray:
            var.stringArray = mScope[op.getId()].stringArray;
            break;
        case ValueType::DoubleArray:
            var.doubleArray = mScope[op.getId()].doubleArray;
            break;
        default:
            throw std::logic_error("invalid var type for access error");
    }
    mStack.push(var);
}

void Interpreter::visit(FunctionCallAST const &op)
{
    if (op.getResultType() == ValueType::Void)
    {
        op.acceptArgument(*this);
        if (mStack.empty())
        {
            throw std::logic_error("invalid stack size for evaluating assignment");
        }
        auto arg = mStack.top();
        mStack.pop();

        printVar(arg, op.getName() == FunctionCallAST::PRINTLN);
    }
    else
    {
        mStack.push(readVar(op.getResultType()));
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

void Interpreter::visit(ArrayAssignmentAST const &op)
{
    op.acceptValue(*this);

    if (mStack.empty())
    {
        throw std::logic_error("invalid stack size for evaluating array assignment");
    }

    auto value = mStack.top();
    mStack.pop();

    ValueType arrayType = mScope[op.getId()].type;
    if (!(isArrayType(arrayType) && (getArrayItemType(arrayType) == value.type)))
    {
        throw std::logic_error("assignment type missmatch");
    }

    auto assigner = [&](auto & collection, auto val) {
        if (op.hasIndex())
        {
            op.acceptIndex(*this);
            if (mStack.empty())
            {
                throw std::logic_error("invalid stack size for evaluating array existing value assignment");
            }

            auto index = mStack.top();
            mStack.pop();
            if (index.type != ValueType::Int)
            {
                throw std::logic_error("invalid index type: " + valueTypeToString(index.type));
            }

            if (index.intVal < 0 || index.intVal >= collection.size())
            {
                throw std::out_of_range(op.getId() + "has not " + std::to_string(index.intVal) + " index");
            }

            collection[index.intVal] = val;
        }
        else
        {
            collection.push_back(val);
        }
    };

    switch (value.type)
    {
        case ValueType::Int:
            assigner(mScope[op.getId()].intArray, value.intVal);
            break;
        case ValueType::String:
            assigner(mScope[op.getId()].stringArray, value.strVal);
            break;
        case ValueType::Bool:
            assigner(mScope[op.getId()].boolArray, value.boolVal);
            break;
        case ValueType::Double:
            assigner(mScope[op.getId()].doubleArray, value.doubleVal);
            break;
        default:
            throw std::logic_error("v to arr, unassignable type error");
    }
}

void Interpreter::visit(ArrayAccessAST const &op)
{
    auto valueRetriever = [&](auto & collection) {
        op.acceptIndex(*this);
        if (mStack.empty())
        {
            throw std::logic_error("invalid stack size for array access");
        }

        auto index = mStack.top();
        mStack.pop();
        if (index.type != ValueType::Int)
        {
            throw std::logic_error("invalid index type: " + valueTypeToString(index.type));
        }

        if (index.intVal < 0 || index.intVal >= collection.size())
        {
            throw std::out_of_range(op.getId() + "has not " + std::to_string(index.intVal) + " index");
        }

        return collection[index.intVal];
    };

    Var var{op.getResultType()};
    switch (var.type)
    {
        case ValueType::Int:
            var.intVal = valueRetriever(mScope[op.getId()].intArray);
            break;
        case ValueType::String:
            var.strVal = valueRetriever(mScope[op.getId()].stringArray);
            break;
        case ValueType::Bool:
            var.boolVal = valueRetriever(mScope[op.getId()].boolArray);
            break;
        case ValueType::Double:
            var.doubleVal = valueRetriever(mScope[op.getId()].doubleArray);
            break;
        default:
            throw std::logic_error("invalid var type for access error");
    }
    mStack.push(var);
}

void Interpreter::visit(BoolAST const &op)
{
    mStack.push(Var{AST::ValueType::Bool, 0, op.getValue()});
}

void Interpreter::printVar(const Interpreter::Var &v, bool addNewLine)
{
    switch (v.type)
    {
        case ValueType::Int:
            mOut << v.intVal;
            break;
        case ValueType::String:
            mOut << v.strVal;
            break;
        case ValueType::Bool:
            mOut << (v.boolVal ? "true" : "false");
            break;
        case ValueType::Double:
            mOut << v.doubleVal;
            break;
        case ValueType::IntArray:
            mOut << arrayToString(v.intArray);
            break;
        case ValueType::BoolArray:
            mOut << arrayToString(v.boolArray);
            break;
        case ValueType::StringArray:
            mOut << arrayToString(v.stringArray);
            break;
        case ValueType::DoubleArray:
            mOut << arrayToString(v.doubleArray);
            break;
        default:
            throw std::logic_error("invalid var type error");
    }

    if (addNewLine)
    {
        mOut << std::endl;
    }
}

Interpreter::Var Interpreter::readVar(AST::ValueType const &t)
{
    Var var{t};
    switch (t)
    {
        case ValueType::Int:
            mIn >> var.intVal;
            break;
        case ValueType::Bool:
            mIn >> var.boolVal;
            break;
        case ValueType::String:
            mIn >> var.strVal;
            break;
        case ValueType::Double:
            mIn >> var.doubleVal;
            break;
        default:
            throw std::logic_error("unexpected value type: " + valueTypeToString(t) + " when executing read");
    }

    return var;
}
