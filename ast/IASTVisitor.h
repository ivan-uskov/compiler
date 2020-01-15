#pragma once

namespace AST
{
    class BinaryOperatorAST;
    class NumberAST;
    class ExpressionPairAST;
    class VariableDeclarationAST;
    class AssignmentAST;
    class VariableAccessAST;
    class FunctionCallAST;
    class BoolBinaryOperatorAST;
    class IfAST;

    class IASTVisitor
    {
    public:
        virtual ~IASTVisitor() = default;
        virtual void visit(BinaryOperatorAST const& op) = 0;
        virtual void visit(NumberAST const& op) = 0;
        virtual void visit(ExpressionPairAST const& op) = 0;
        virtual void visit(VariableDeclarationAST const& op) = 0;
        virtual void visit(AssignmentAST const& op) = 0;
        virtual void visit(VariableAccessAST const& op) = 0;
        virtual void visit(FunctionCallAST const& op) = 0;
        virtual void visit(BoolBinaryOperatorAST const& op) = 0;
        virtual void visit(IfAST const& op) = 0;
    };
}