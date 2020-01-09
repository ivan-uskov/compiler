#pragma once

namespace AST
{
    class BinaryOperatorAST;
    class NumberAST;
    class ExpressionPairAST;
    class VariableDeclarationAST;

    class IASTVisitor
    {
    public:
        virtual ~IASTVisitor() = default;
        virtual void visit(BinaryOperatorAST const& op) = 0;
        virtual void visit(NumberAST const& op) = 0;
        virtual void visit(ExpressionPairAST const& op) = 0;
        virtual void visit(VariableDeclarationAST const& op) = 0;
    };
}