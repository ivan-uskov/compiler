#pragma once

#include <stack>
#include <vector>
#include "ast/IASTVisitor.h"
#include "ast/ValueType.h"

#include <map>

namespace Translation
{
    class Interpreter : public AST::IASTVisitor
    {
    public:
        explicit Interpreter(std::ostream & out);

        void visit(AST::BinaryOperatorAST const& op) override;
        void visit(AST::NumberAST const& op) override;
        void visit(const AST::ExpressionPairAST &op) override;
        void visit(const AST::VariableDeclarationAST &op) override;
        void visit(const AST::AssignmentAST &op) override;
        void visit(const AST::VariableAccessAST &op) override;
        void visit(const AST::FunctionCallAST &op) override;
        void visit(const AST::BoolBinaryOperatorAST &op) override;
        void visit(const AST::IfAST &op) override;
        void visit(const AST::WhileAST &op) override;

    private:
        struct Var
        {
            AST::ValueType type;
            std::string strVal;
            double numVal = 0;
        };

        std::stack<double> mStack;
        std::map<std::string, Var> mScope;

        std::ostream & mOut;
    };
}