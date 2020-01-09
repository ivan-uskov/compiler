#include "../FunctionCallAST.h"

AST::FunctionCallAST::FunctionCallAST(std::unique_ptr<IAST> &&arg)
    : mArg(std::move(arg))
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
