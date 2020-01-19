#include "../FunctionCallAST.h"

#include <utility>

const std::string AST::FunctionCallAST::READ = "read";
const std::string AST::FunctionCallAST::PRINT = "print";
const std::string AST::FunctionCallAST::PRINTLN = "println";

AST::FunctionCallAST::FunctionCallAST(std::string name, std::unique_ptr<IAST> &&arg)
    : mArg(std::move(arg))
    , mName(std::move(name))
{
}

void AST::FunctionCallAST::accept(AST::IASTVisitor &v) const
{
    v.visit(*this);
}

AST::ValueType AST::FunctionCallAST::getResultType() const
{
    return Void;
}

void AST::FunctionCallAST::acceptArgument(AST::IASTVisitor &v) const
{
    mArg->accept(v);
}

bool AST::FunctionCallAST::isSupportedFunctionName(std::string const &name)
{
    return name == PRINT || name == PRINTLN;
}

std::string AST::FunctionCallAST::getName() const
{
    return mName;
}
