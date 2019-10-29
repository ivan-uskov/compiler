#include "../ASTView.h"
#include "../BinaryOperatorAST.h"
#include "../NumberAST.h"

ASTView::ASTView(std::ostream & out)
    : mOut(out)
{}

void ASTView::visit(BinaryOperatorAST const& op)
{
    mOut << "(";
    op.acceptLeft(*this);
    mOut << op.getType();
    op.acceptRight(*this);
    mOut << ")";
}

void ASTView::visit(NumberAST const& op)
{
    mOut << op.getValue();
}