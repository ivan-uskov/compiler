#include "../FunctionCallAST.h"

#include <utility>

const std::string AST::FunctionCallAST::READ_BOOL = "readBool";
const std::string AST::FunctionCallAST::READ_INT = "readInt";
const std::string AST::FunctionCallAST::READ_DOUBLE = "readDouble";
const std::string AST::FunctionCallAST::READ_STRING = "readString";
const std::string AST::FunctionCallAST::PRINT = "print";
const std::string AST::FunctionCallAST::PRINTLN = "println";

AST::FunctionCallAST::FunctionCallAST(std::string name, std::unique_ptr<IAST> &&arg)
    : mArg(std::move(arg))
    , mName(std::move(name))
{
    if (!isPrintFunctionName(mName))
    {
        throw std::logic_error("invalid function name for print fn: " + name);
    }
}

AST::FunctionCallAST::FunctionCallAST(std::string name)
    : mName(std::move(name))
{
    if (!isReadFunctionName(mName))
    {
        throw std::logic_error("invalid function name for read fn: " + name);
    }
}

void AST::FunctionCallAST::accept(AST::IASTVisitor &v) const
{
    v.visit(*this);
}

AST::ValueType AST::FunctionCallAST::getResultType() const
{
    if (mName == READ_STRING)
    {
        return ValueType::String;
    }
    if (mName == READ_INT)
    {
        return ValueType::Int;
    }
    if (mName == READ_DOUBLE)
    {
        return ValueType::Double;
    }
    if (mName == READ_BOOL)
    {
        return ValueType::Bool;
    }

    return Void;
}

void AST::FunctionCallAST::acceptArgument(AST::IASTVisitor &v) const
{
    if (mArg == nullptr)
    {
        throw std::logic_error("tring acces to null argument on " + mName + " fn");
    }

    mArg->accept(v);
}

bool AST::FunctionCallAST::isSupportedFunctionName(std::string const &name)
{
    return isPrintFunctionName(name) || isReadFunctionName(name);
}

std::string AST::FunctionCallAST::getName() const
{
    return mName;
}

bool AST::FunctionCallAST::isReadFunctionName(std::string const &name)
{
    return name == READ_BOOL || name == READ_INT || name == READ_DOUBLE || name == READ_STRING;
}

bool AST::FunctionCallAST::isPrintFunctionName(std::string const &name)
{
    return name == PRINT || name == PRINTLN;
}
