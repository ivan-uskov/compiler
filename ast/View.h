#pragma once
#include <ostream>

#include "IASTVisitor.h"

namespace AST
{
    class View : public IASTVisitor
    {
    public:
        explicit View(std::ostream & out);

        void visit(NumberBinaryOperatorAST const& op) override;
        void visit(IntAST const& op) override;
        void visit(ExpressionPairAST const &op) override;
        void visit(VariableDeclarationAST const &op) override;
        void visit(AssignmentAST const &op) override;
        void visit(VariableAccessAST const &op) override;
        void visit(FunctionCallAST const &op) override;
        void visit(CompareBinaryOperatorAST const &op) override;
        void visit(IfAST const &op) override;
        void visit(WhileAST const &op) override;
        void visit(StringAST const &op) override;
        void visit(DoubleAST const &op) override;
        void visit(ArrayAssignmentAST const &op) override;
        void visit(ArrayAccessAST const &op) override;

    private:
        std::ostream & mOut;
    };
}