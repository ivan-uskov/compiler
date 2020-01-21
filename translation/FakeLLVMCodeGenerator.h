#pragma once

#include "ast/IASTVisitor.h"
#include "ast/IAST.h"

#include <sstream>

namespace Translation {
    class FakeLLVMCodeGenerator : public AST::IASTVisitor {
    public:
        std::string generate(AST::IAST const& ast);

        void visit(const AST::NumberBinaryOperatorAST &op) override;
        void visit(const AST::IntAST &op) override;
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
        void visit(const AST::ArrayAssignmentAST &op) override;
        void visit(const AST::ArrayAccessAST &op) override;
        void visit(const AST::BoolAST &op) override;

    private:
        std::ostringstream mAcc;
    };
}