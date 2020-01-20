#include "../IfAST.h"

void AST::IfAST::accept(AST::IASTVisitor &v) const
{
    v.visit(*this);
}

AST::ValueType AST::IfAST::getResultType() const
{
    return Void;
}

AST::IfAST::IfAST(std::unique_ptr<IAST> &&cond, std::unique_ptr<IAST> &&stmt)
    : mCond(std::move(cond))
    , mStmt(std::move(stmt))
{
    if (mCond->getResultType() != ValueType::Bool)
    {
        throw std::logic_error("invalid condition type when construct if ast");
    }
}

void AST::IfAST::acceptCond(AST::IASTVisitor &v) const
{
    mCond->accept(v);
}

void AST::IfAST::acceptStmt(AST::IASTVisitor &v) const
{
    mStmt->accept(v);
}