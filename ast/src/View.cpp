#include "ast/View.h"
#include "../BinaryOperatorAST.h"
#include "../NumberAST.h"

using namespace AST;

View::View(std::ostream & out)
    : mOut(out)
{}

void View::visit(BinaryOperatorAST const& op)
{
    mOut << "(";
    op.acceptLeft(*this);
    mOut << op.getType();
    op.acceptRight(*this);
    mOut << ")";
}

void View::visit(NumberAST const& op)
{
    mOut << op.getValue();
}