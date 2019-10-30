#pragma once
#include <ostream>

#include "IASTVisitor.h"

namespace AST
{
    class View : public IASTVisitor
    {
    public:
        explicit View(std::ostream & out);

        void visit(BinaryOperatorAST const& op);
        void visit(NumberAST const& op);
    private:
        std::ostream & mOut;
    };
}