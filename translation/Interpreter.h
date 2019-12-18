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

        std::vector<float> getValues() const;

    private:
        std::stack<float> mStack;
    };
}