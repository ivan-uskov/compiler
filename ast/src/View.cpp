#include "ast/View.h"
#include "ast/NumberBinaryOperatorAST.h"
#include "ast/IntAST.h"
#include "../ExpressionPairAST.h"
#include "../VariableDeclarationAST.h"
#include "../AssignmentAST.h"
#include "../VariableAccessAST.h"
#include "../FunctionCallAST.h"
#include "ast/CompareBinaryOperatorAST.h"
#include "../IfAST.h"
#include "../WhileAST.h"
#include "../StringAST.h"

using namespace AST;

View::View(std::ostream & out)
    : mOut(out)
{}

void View::visit(NumberBinaryOperatorAST const& op)
{
    mOut << "(";
    op.acceptLeft(*this);
    mOut << op.getType();
    op.acceptRight(*this);
    mOut << ")";
}

void View::visit(IntAST const& op)
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

void View::visit(CompareBinaryOperatorAST const &op)
{
    mOut << "(";
    op.acceptLeft(*this);
    mOut << op.getType();
    op.acceptRight(*this);
    mOut << ")";
}

void View::visit(IfAST const &op)
{
    mOut << "if (";
    op.acceptCond(*this);
    mOut << ")" << std::endl;
    mOut << "{" << std::endl << "    ";
    op.acceptStmt(*this);
    mOut << std::endl << "}" << std::endl; //TODO: add spaces
}

void View::visit(WhileAST const &op)
{
    mOut << "while (";
    op.acceptCond(*this);
    mOut << ")" << std::endl;
    mOut << "{" << std::endl << "    ";
    op.acceptStmt(*this);
    mOut << std::endl << "}" << std::endl; //TODO: add spaces
}

void View::visit(StringAST const &op)
{
    mOut << op.getValue();
}