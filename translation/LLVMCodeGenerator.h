#pragma once

#include "ast/IAST.h"
#include "ast/IASTVisitor.h"

#include <stack>
#include <vector>

#include "begin_llvm.h"
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include "end_llvm.h"

namespace Translation
{
    class LLVMCodeGenerator : public AST::IASTVisitor
    {
    public:
        LLVMCodeGenerator();

        std::string generate(AST::IAST & ast);

        void visit(AST::BinaryOperatorAST const& op) override;
        void visit(AST::NumberAST const& op) override;

        void visit(const AST::ExpressionPairAST &op) override;

        void visit(const AST::VariableDeclarationAST &op) override;

    private:
        std::vector<llvm::Value *> getValues() const;
        llvm::Function * GenerateMainDeclaration();
        void GenerateMainDefinition(llvm::Function &fn, AST::IAST & ast);

    private:
        std::unique_ptr<llvm::LLVMContext> mLLVMContext;
        std::unique_ptr<llvm::Module> mModule;
        llvm::IRBuilder<> m_builder;
        llvm::Function * mPrint;

    private:
        std::stack<llvm::Value *> mStack;
    };
}