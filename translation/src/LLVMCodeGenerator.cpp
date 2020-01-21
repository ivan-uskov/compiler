#include "../LLVMCodeGenerator.h"

#include "../begin_llvm.h"
#include <llvm/IR/Constants.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/raw_os_ostream.h>
#include "../end_llvm.h"

#include "ast/IntAST.h"
#include "ast/NumberBinaryOperatorAST.h"
#include "ast/ExpressionPairAST.h"
#include "ast/VariableDeclarationAST.h"
#include "ast/VariableAccessAST.h"
#include "ast/FunctionCallAST.h"
#include "ast/AssignmentAST.h"
#include "ast/CompareBinaryOperatorAST.h"
#include "ast/IfAST.h"
#include "ast/WhileAST.h"
#include "ast/StringAST.h"
#include "ast/DoubleAST.h"
#include "ast/ArrayAssignmentAST.h"
#include "ast/ArrayAccessAST.h"
#include "ast/BoolAST.h"
#include "ast/ValueType.h"

using namespace std;
using namespace llvm;
using namespace Translation;

LLVMCodeGenerator::LLVMCodeGenerator()
    : mLLVMContext(new llvm::LLVMContext)
    , mModule(new llvm::Module("main module", *mLLVMContext))
    , mBuilder(*mLLVMContext)
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
    Value *GenerateNumberBinaryExpr(IRBuilder<> & builder, LLVMContext &context, Value *a, AST::NumberBinaryOperatorAST::Type t, Value *b, bool isInt = false)
    {
        switch (t)
        {
            case AST::NumberBinaryOperatorAST::Type::Sum:
                return isInt ? builder.CreateAdd(a, b, "addtmp") : builder.CreateFAdd(builder.CreateFPCast(a, a->getType()), builder.CreateFPCast(b, b->getType()), "addtmp");
            case AST::NumberBinaryOperatorAST::Type::Sub:
                return isInt ? builder.CreateSub(a, b, "subtmp") : builder.CreateFSub(builder.CreateFPCast(a, a->getType()), builder.CreateFPCast(b, b->getType()), "subtmp");
            case AST::NumberBinaryOperatorAST::Type::Mul:
                return isInt ? builder.CreateMul(a, b, "multmp") : builder.CreateFMul(builder.CreateFPCast(a, a->getType()), builder.CreateFPCast(b, b->getType()), "multmp");
            case AST::NumberBinaryOperatorAST::Type::Div:
                return isInt ? builder.CreateFDiv(a, b, "divtmp") : builder.CreateFDiv(builder.CreateFPCast(a, a->getType()), builder.CreateFPCast(b, b->getType()), "divtmp");
        }
    }

    Value *GenerateCompareBinaryExpr(IRBuilder<> & builder, LLVMContext &context, Value *a, AST::CompareBinaryOperatorAST::Type t, Value *b, bool isInt = false)
    {
        // Convert bool 0/1 to double 0.0 or 1.0
        switch (t)
        {
            case AST::CompareBinaryOperatorAST::Type::Less:
            {
                Value * temp = isInt ? builder.CreateICmpSLT(a, b, "cmptmp") : builder.CreateFCmpULT(a, b, "cmptmp");
                return builder.CreateUIToFP(temp, Type::getDoubleTy(context), "booltmp");
            }
            case AST::CompareBinaryOperatorAST::Type::More:
            {
                Value *temp = isInt ? builder.CreateICmpSLT(b, a, "cmptmp") : builder.CreateFCmpULT(b, a, "cmptmp");
                return builder.CreateUIToFP(temp, Type::getDoubleTy(context), "booltmp");
            }
            case AST::CompareBinaryOperatorAST::Type::Equals:
            {
                Value *temp = isInt ? builder.CreateICmpEQ(a, b, "cmptmp") : builder.CreateFCmpUEQ(a, b, "cmptmp");
                return builder.CreateUIToFP(temp, Type::getDoubleTy(context), "booltmp");
            }
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
    mBuilder.SetInsertPoint(bb);

    ast.accept(*this);

    for (auto v : getValues())
    {
        Constant* pFormatAddress = AddStringLiteral(*mLLVMContext, *mModule, "%lf\n");
        std::vector<llvm::Value *> args = {pFormatAddress, v};
        mBuilder.CreateCall(mPrint, args);
    }

    Constant *exitCode = ConstantInt::get(*mLLVMContext, APInt(32, uint64_t(0), true));
    mBuilder.CreateRet(exitCode);

    std::string outputStr;
    raw_string_ostream output(outputStr);
    if (verifyFunction(fn, &output))
    {
        throw logic_error("Function verification failed for main '" + output.str() + "'");
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
    auto right = mStack.top();
    mStack.pop();
    auto left = mStack.top();
    mStack.pop();

    mStack.push(GenerateNumberBinaryExpr(mBuilder, *mLLVMContext, left, op.getType(), right, op.getResultType() == AST::ValueType::Int));
}

void LLVMCodeGenerator::visit(AST::IntAST const &op)
{
    mStack.push(ConstantInt::get(*mLLVMContext, APSInt(op.getValue())));
}

void LLVMCodeGenerator::visit(const AST::VariableDeclarationAST &op)
{
    // skip
}

void LLVMCodeGenerator::visit(const AST::AssignmentAST &op)
{
    op.acceptValue(*this);
    auto val = mStack.top();
    mStack.pop();

    mScope[op.getId()] = val;
}

void LLVMCodeGenerator::visit(const AST::VariableAccessAST &op)
{
    mStack.push(mScope[op.getId()]);
}

void LLVMCodeGenerator::visit(const AST::FunctionCallAST &op)
{
    if (op.getResultType() == AST::ValueType::Void)
    {
        auto format = [&op] () -> std::string {
            switch (op.getArgumentType())
            {
                case AST::ValueType::Int:
                case AST::ValueType::Bool:
                    return "%ld";
                case AST::ValueType::Double:
                    return "%lf";
                case AST::ValueType::String:
                    return "%s";
                default:
                    return "";
                    //TODO: add array print
            }
        };

        op.acceptArgument(*this);
        auto v = mStack.top();
        mStack.pop();

        Constant* pFormatAddress = AddStringLiteral(*mLLVMContext, *mModule, format() + (op.getName() == AST::FunctionCallAST::PRINTLN ? "\n" : ""));
        std::vector<llvm::Value *> args = {pFormatAddress, v};
        mBuilder.CreateCall(mPrint, args);
    }
    else
    {
        //TODO: add read fn
    }
}

void LLVMCodeGenerator::visit(const AST::CompareBinaryOperatorAST &op)
{
    op.acceptLeft(*this);
    op.acceptRight(*this);
    auto right = mStack.top();
    mStack.pop();
    auto left = mStack.top();
    mStack.pop();

    auto isInt = (op.getLeftType() == op.getRightType()) && (op.getLeftType() == AST::ValueType::Int);

    mStack.push(GenerateCompareBinaryExpr(mBuilder, *mLLVMContext, left, op.getType(), right, isInt));
}

void LLVMCodeGenerator::visit(const AST::IfAST &op)
{
    Function *function = mBuilder.GetInsertBlock()->getParent();
    BasicBlock *thenBB = BasicBlock::Create(*mLLVMContext, "then", function);
    BasicBlock *elseBB = BasicBlock::Create(*mLLVMContext, "else", function);
    BasicBlock *mergeBB = BasicBlock::Create(*mLLVMContext, "merge_if", function);

    op.acceptCond(*this);
    auto cond = popStack();

    mBuilder.CreateCondBr(cond, thenBB, elseBB);

    mBuilder.SetInsertPoint(thenBB);
    op.acceptStmt(*this);
    mBuilder.CreateBr(mergeBB);

    mBuilder.SetInsertPoint(elseBB);
    mBuilder.CreateBr(mergeBB);

    mBuilder.SetInsertPoint(mergeBB);
}

void LLVMCodeGenerator::visit(const AST::WhileAST &op)
{
    Function *function = mBuilder.GetInsertBlock()->getParent();
    BasicBlock *conditionBB = BasicBlock::Create(*mLLVMContext, "cond", function);
    BasicBlock *loopBB = BasicBlock::Create(*mLLVMContext, "loop", function);
    BasicBlock *nextBB = BasicBlock::Create(*mLLVMContext, "after_loop", function);

    mBuilder.CreateBr(conditionBB);
    mBuilder.SetInsertPoint(conditionBB);

    op.acceptCond(*this);
    auto condition = popStack();

    mBuilder.CreateCondBr(condition, loopBB, nextBB);
    mBuilder.SetInsertPoint(loopBB);
    op.acceptStmt(*this);
    mBuilder.CreateBr(conditionBB);

    mBuilder.SetInsertPoint(nextBB);
}

void LLVMCodeGenerator::visit(const AST::StringAST &op)
{
    mStack.push(AddStringLiteral(*mLLVMContext, *mModule, op.getValue()));
}

void LLVMCodeGenerator::visit(const AST::DoubleAST &op)
{
    mStack.push(ConstantFP::get(*mLLVMContext, APFloat(op.getValue())));
}

void LLVMCodeGenerator::visit(const AST::ArrayAssignmentAST &op)
{
    //TODO: impl
}

void LLVMCodeGenerator::visit(const AST::ArrayAccessAST &op)
{
    //TODO: impl
}

void LLVMCodeGenerator::visit(const AST::BoolAST &op)
{
    mStack.push(ConstantInt::get(*mLLVMContext, APInt(8, uint64_t(op.getValue()), true)));
}

llvm::Value *LLVMCodeGenerator::popStack()
{
    auto val = mStack.top();
    mStack.pop();

    return val;
}
