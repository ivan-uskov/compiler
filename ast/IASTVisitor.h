#pragma once

namespace AST
{
    class NumberBinaryOperatorAST;
    class IntAST;
    class ExpressionPairAST;
    class VariableDeclarationAST;
    class AssignmentAST;
    class VariableAccessAST;
    class FunctionCallAST;
    class CompareBinaryOperatorAST;
    class IfAST;
    class WhileAST;
    class StringAST;

    class IASTVisitor
    {
    public:
        virtual ~IASTVisitor() = default;
        virtual void visit(NumberBinaryOperatorAST const& op) = 0;
        virtual void visit(IntAST const& op) = 0;
        virtual void visit(ExpressionPairAST const& op) = 0;
        virtual void visit(VariableDeclarationAST const& op) = 0;
        virtual void visit(AssignmentAST const& op) = 0;
        virtual void visit(VariableAccessAST const& op) = 0;
        virtual void visit(FunctionCallAST const& op) = 0;
        virtual void visit(CompareBinaryOperatorAST const& op) = 0;
        virtual void visit(IfAST const& op) = 0;
        virtual void visit(WhileAST const& op) = 0;
        virtual void visit(StringAST const& op) = 0;
    };
}