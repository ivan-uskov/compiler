#include "../WhileAST.h"

void AST::WhileAST::accept(AST::IASTVisitor &v) const
{
    v.visit(*this);
}

AST::ValueType AST::WhileAST::getResultType() const
{
    return Void;
}

AST::WhileAST::WhileAST(std::unique_ptr<IAST> &&cond, std::unique_ptr<IAST> &&stmt)
    : mCond(std::move(cond))
    , mStmt(std::move(stmt))
{
    if (mCond->getResultType() != ValueType::Bool)
    {
        throw std::logic_error("invalid condition type when construct while ast");
    }
}

void AST::WhileAST::acceptCond(AST::IASTVisitor &v) const
{
    mCond->accept(v);
}

void AST::WhileAST::acceptStmt(AST::IASTVisitor &v) const
{
    mStmt->accept(v);
}