#pragma once
#include <ostream>

#include "IASTVisitor.h"

namespace AST
{
    class View : public IASTVisitor
    {
    public:
        explicit View(std::ostream & out);

        void visit(BinaryOperatorAST const& op) override;
        void visit(NumberAST const& op) override;

        void visit(ExpressionPairAST const &op) override;

    private:
        std::ostream & mOut;
    };
}