#pragma once

#include <stack>
#include <vector>
#include "ast/IASTVisitor.h"

namespace Translation
{
    class Interpreter : public AST::IASTVisitor
    {
    public:
        void visit(AST::BinaryOperatorAST const& op) override;
        void visit(AST::NumberAST const& op) override;
        void visit(const AST::ExpressionPairAST &op) override;
        void visit(const AST::VariableDeclarationAST &op) override;
        void visit(const AST::AssignmentAST &op) override;
        void visit(const AST::VariableAccessAST &op) override;
        void visit(const AST::FunctionCallAST &op) override;

        std::vector<double> getValues() const;

    private:
        std::stack<double> mStack;
    };
}