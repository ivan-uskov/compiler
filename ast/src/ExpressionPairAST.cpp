#include "../ExpressionPairAST.h"

using namespace AST;

ExpressionPairAST::ExpressionPairAST(std::unique_ptr<IAST> &&left, std::unique_ptr<IAST> &&right)
    : mLeft(std::move(left))
    , mRight(std::move(right))
{
}


void ExpressionPairAST::accept(IASTVisitor & v) const
{
    v.visit(*this);
}

void ExpressionPairAST::acceptLeft(IASTVisitor & v) const
{
    mLeft->accept(v);
}

void ExpressionPairAST::acceptRight(IASTVisitor & v) const
{
    mRight->accept(v);
}

ValueType ExpressionPairAST::getResultType() const
{
    return Void;
}
