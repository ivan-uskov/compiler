#include "../LLVMCodeGenerator.h"

#include "../begin_llvm.h"
#include <llvm/IR/Constants.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/raw_os_ostream.h>
#include "../end_llvm.h"

#include "ast/NumberBinaryOperatorAST.h"
#include "ast/IntAST.h"
#include "ast/ExpressionPairAST.h"

using namespace std;
using namespace llvm;
using namespace Translation;

LLVMCodeGenerator::LLVMCodeGenerator()
    : mLLVMContext(new llvm::LLVMContext)
    , mModule(new llvm::Module("main module", *mLLVMContext))
    , m_builder(*mLLVMContext)
{
    auto & context = *mLLVMContext;
    std::vector<llvm::Type *> argTypes = {llvm::Type::getInt8PtrTy(context)};
    llvm::FunctionType *fnType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), argTypes, true);

    mPrint = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, "printf", mModule.get());
}

std::vector<llvm::Value *> LLVMCodeGenerator::getValues() const
{
    auto stack = mStack;
    std::vector<llvm::Value *> res;
    res.reserve(stack.size());
    while (!stack.empty())
    {
        res.push_back(stack.top());
        stack.pop();
    }
    std::reverse(res.begin(), res.end());
    return res;

}

namespace
{
    Value *GenerateBinaryExpr(IRBuilder<> & builder, LLVMContext &context, Value *a, AST::NumberBinaryOperatorAST::Type t, Value *b)
    {
        switch (t)
        {
            case AST::NumberBinaryOperatorAST::Type::Sum:
                return builder.CreateFAdd(a, b, "addtmp");
            case AST::NumberBinaryOperatorAST::Type::Sub:
                return builder.CreateFSub(a, b, "subtmp");
            case AST::NumberBinaryOperatorAST::Type::Mul:
                return builder.CreateFMul(a, b, "multmp");
            case AST::NumberBinaryOperatorAST::Type::Div:
                return builder.CreateFDiv(a, b, "divtmp");
        }
    }

    Constant *AddStringLiteral(LLVMContext & context, Module & module, std::string const& value)
    {
        Constant *pConstant = ConstantDataArray::getString(context, value, true);
        auto * global = new GlobalVariable(module, pConstant->getType(), true,
                                                    GlobalValue::InternalLinkage, pConstant, "str");

        Constant *index = Constant::getNullValue(IntegerType::getInt32Ty(context));
        std::vector<Constant*> indices = {index, index};

        return ConstantExpr::getInBoundsGetElementPtr(pConstant->getType(), global, indices);
    }
}

void LLVMCodeGenerator::visit(const AST::ExpressionPairAST &op)
{
    op.acceptLeft(*this);
    op.acceptRight(*this);
}

void LLVMCodeGenerator::visit(AST::NumberBinaryOperatorAST const &op)
{
    op.acceptLeft(*this);
    op.acceptRight(*this);
    Value *right = mStack.top();
    mStack.pop();
    Value *left = mStack.top();
    mStack.pop();

    auto pValue = GenerateBinaryExpr(m_builder, *mLLVMContext, left, op.getType(), right);
    mStack.push(pValue);
}

void LLVMCodeGenerator::visit(AST::IntAST const &op)
{
    mStack.push(ConstantInt::get(*mLLVMContext, APSInt(op.getValue())));
}

std::string LLVMCodeGenerator::generate(AST::IAST & ast)
{
    Function *fn = GenerateMainDeclaration();
    GenerateMainDefinition(*fn, ast);

    ostringstream ostrm;
    llvm::raw_os_ostream stream(ostrm);
    mModule->print(stream, nullptr);

    return ostrm.str();
}

Function * LLVMCodeGenerator::GenerateMainDeclaration()
{
    std::vector<Type *> args;
    Type *returnType = Type::getInt32Ty(*mLLVMContext);

    FunctionType *fnType = FunctionType::get(returnType, args, false);
    std::string fnName = "main";
    return Function::Create(fnType, Function::ExternalLinkage, fnName, mModule.get());
}

void LLVMCodeGenerator::GenerateMainDefinition(Function &fn, AST::IAST & ast)
{
    auto & context = *mLLVMContext;

    // Создаём базовый блок CFG для вставки инструкций в этот блок.
    BasicBlock *bb = BasicBlock::Create(context, "entry", &fn);
    m_builder.SetInsertPoint(bb);

    ast.accept(*this);

    for (auto v : getValues())
    {
        Constant* pFormatAddress = AddStringLiteral(*mLLVMContext, *mModule, "%lf\n");
        std::vector<llvm::Value *> args = {pFormatAddress, v};
        m_builder.CreateCall(mPrint, args);
    }

    Constant *exitCode = ConstantInt::get(*mLLVMContext, APInt(32, uint64_t(0), true));
    m_builder.CreateRet(exitCode);

    std::string outputStr;
    raw_string_ostream output(outputStr);
    if (verifyFunction(fn, &output))
    {
        throw logic_error("Function verification failed for main '" + output.str() + "'");
    }
}

void LLVMCodeGenerator::visit(const AST::VariableDeclarationAST &op)
{
    //TODO: add impl
}

void LLVMCodeGenerator::visit(const AST::AssignmentAST &op)
{
    //TODO: add impl
}

void LLVMCodeGenerator::visit(const AST::VariableAccessAST &op)
{
    //TODO: add impl
}

void LLVMCodeGenerator::visit(const AST::FunctionCallAST &op)
{
    //TODO: add impl
}

void LLVMCodeGenerator::visit(const AST::CompareBinaryOperatorAST &op)
{
    //TODO: add impl
}

void LLVMCodeGenerator::visit(const AST::IfAST &op)
{
    //TODO: impl
}

void LLVMCodeGenerator::visit(const AST::WhileAST &op)
{
    //TODO: impl
}

void LLVMCodeGenerator::visit(const AST::StringAST &op)
{
    //TODO: impl
}

void LLVMCodeGenerator::visit(const AST::DoubleAST &op)
{
    //TODO: impl
}

void LLVMCodeGenerator::visit(const AST::ArrayAssignmentAST &op)
{
    //TODO: impl
}
