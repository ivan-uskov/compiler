#include "../FakeLLVMCodeGenerator.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "ast/NumberBinaryOperatorAST.h"
#include "ast/IntAST.h"
#include "ast/ExpressionPairAST.h"
#include "ast/VariableDeclarationAST.h"
#include "ast/AssignmentAST.h"
#include "ast/VariableAccessAST.h"
#include "ast/FunctionCallAST.h"
#include "ast/CompareBinaryOperatorAST.h"
#include "ast/IfAST.h"
#include "ast/WhileAST.h"
#include "ast/StringAST.h"
#include "ast/DoubleAST.h"
#include "ast/ArrayAssignmentAST.h"
#include "ast/ArrayAccessAST.h"
#include "ast/BoolAST.h"

using namespace std;
using namespace AST;

std::string Translation::FakeLLVMCodeGenerator::generate(AST::IAST const& ast)
{
    mAcc << R"(
#include <iostream>
#include <vector>

using namespace std;

using IntArray = vector<int>;
using BoolArray = vector<bool>;
using DoubleArray = vector<double>;
using StringArray = vector<string>;

int readInt()
{
    int a;
    cin >> a;
    return a;
}
bool readBool()
{
    bool a;
    cin >> a;
    return a;
}
string readString()
{
    string a;
    cin >> a;
    return a;
}
double readDouble()
{
    double a;
    cin >> a;
    return a;
}

void print(bool v)
{
    cout << (v ? "true" : "false");
}

template <typename T>
void print(T v)
{
    cout << v;
}

template<typename T>
void print(vector<T> arr) {
    cout << "{";
    for (size_t i = 0; i < arr.size(); ++i)
    {
        cout << arr[i];
        if (i < arr.size() - 1)
        {
            cout << ", ";
        }
    }
    cout << "}";
}

template <typename T>
void println(T v)
{
    print(v);
    cout << endl;
}

int main() {
)";

    ast.accept(*this);
    mAcc << R"(
    return 0;
}
)";

    {
        std::ofstream tmpIn("tmpIn.cpp");
        tmpIn << mAcc.str() << endl;
        tmpIn.close();
    }
    system("clang-9 -S -emit-llvm tmpIn.cpp");
    std::ifstream tmpLL("tmpIn.ll", std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(tmpLL)),std::istreambuf_iterator<char>());

    remove("tmpIn.cpp");
    remove("tmpIn.ll");

    return str;
}


void Translation::FakeLLVMCodeGenerator::visit(const AST::NumberBinaryOperatorAST &op)
{
    mAcc << "(";
    op.acceptLeft(*this);
    mAcc << op.getType();
    op.acceptRight(*this);
    mAcc << ")";
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::IntAST &op)
{
    mAcc << op.getValue();
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::ExpressionPairAST &op)
{
    op.acceptLeft(*this);
    op.acceptRight(*this);
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::VariableDeclarationAST &op)
{
    mAcc << op.getResultType() << " " << op.getId() << ";" << endl;
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::AssignmentAST &op)
{
    mAcc << op.getId() << " = ";
    op.acceptValue(*this);
    mAcc << ";" << std::endl;
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::VariableAccessAST &op)
{
    mAcc << op.getId();
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::FunctionCallAST &op)
{
    mAcc << op.getName() << "(";
    if (op.getResultType() == ValueType::Void)
    {
        op.acceptArgument(*this);
        mAcc << ")" << ";" << std::endl;
    }
    else
    {
        mAcc << ")";
    }
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::CompareBinaryOperatorAST &op)
{
    mAcc << "(";
    op.acceptLeft(*this);
    mAcc << op.getType();
    op.acceptRight(*this);
    mAcc << ")";
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::IfAST &op)
{
    mAcc << "if (";
    op.acceptCond(*this);
    mAcc << ")" << std::endl;
    mAcc << "{" << std::endl;
    op.acceptStmt(*this);
    mAcc << std::endl << "}" << std::endl;
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::WhileAST &op)
{
    mAcc << "while (";
    op.acceptCond(*this);
    mAcc << ")" << std::endl;
    mAcc << "{" << std::endl;
    op.acceptStmt(*this);
    mAcc << std::endl << "}" << std::endl;
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::StringAST &op)
{
    mAcc << "string(\"" << op.getValue() << "\")";
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::DoubleAST &op)
{
    mAcc << op.getValue();
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::ArrayAssignmentAST &op)
{
    if (op.hasIndex())
    {
        mAcc << op.getId() << "[";
        op.acceptIndex(*this);
        mAcc << "] = ";
        op.acceptValue(*this);
    }
    else
    {
        mAcc << op.getId() << ".push_back(";
        op.acceptValue(*this);
        mAcc << ")";
    }

    mAcc << ";" << endl;
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::ArrayAccessAST &op)
{
    mAcc << op.getId() << "[";
    op.acceptIndex(*this);
    mAcc << "]";
}

void Translation::FakeLLVMCodeGenerator::visit(const AST::BoolAST &op)
{
    mAcc << (op.getValue() ? "true" : "false");
}