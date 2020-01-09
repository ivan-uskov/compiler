#include "ast/View.h"
#include "../BinaryOperatorAST.h"
#include "../NumberAST.h"
#include "../ExpressionPairAST.h"
#include "../VariableDeclarationAST.h"
#include "../AssignmentAST.h"
#include "../VariableAccessAST.h"
#include "../FunctionCallAST.h"

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

void View::visit(ExpressionPairAST const &op)
{
    op.acceptLeft(*this);
    mOut << ";" << std::endl;
    op.acceptRight(*this);
}

void View::visit(VariableDeclarationAST const &op)
{
    mOut << op.getResultType() << " " << op.getId();
}

void View::visit(AssignmentAST const &op)
{
    mOut << op.getId() << " = ";
    op.acceptValue(*this);
}

void View::visit(VariableAccessAST const &op)
{
    mOut << op.getId();
}

void View::visit(FunctionCallAST const &op)
{
    mOut << "print(";
    op.acceptArgument(*this);
    mOut << ")";
}
