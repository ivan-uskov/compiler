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
        explicit Interpreter(std::istream & in, std::ostream & out);

        void visit(AST::NumberBinaryOperatorAST const& op) override;
        void visit(AST::IntAST const& op) override;
        void visit(const AST::ExpressionPairAST &op) override;
        void visit(const AST::VariableDeclarationAST &op) override;
        void visit(const AST::AssignmentAST &op) override;
        void visit(const AST::VariableAccessAST &op) override;
        void visit(const AST::FunctionCallAST &op) override;
        void visit(const AST::CompareBinaryOperatorAST &op) override;
        void visit(const AST::IfAST &op) override;
        void visit(const AST::WhileAST &op) override;
        void visit(const AST::StringAST &op) override;

        void visit(const AST::DoubleAST &op) override;

    private:
        struct Var
        {
            AST::ValueType type;
            int intVal = 0;
            bool boolVal = false;
            std::string strVal;
            double doubleVal = 0.0;
        };

        std::stack<Var> mStack;
        std::map<std::string, Var> mScope;

        std::istream & mIn;
        std::ostream & mOut;
    };
}