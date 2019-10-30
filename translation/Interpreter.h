#pragma once

#include <stack>
#include "ast/IASTVisitor.h"

class Interpreter : public AST::IASTVisitor
{
public:
    void visit(AST::BinaryOperatorAST const& op) override;
    void visit(AST::NumberAST const& op) override;

    float getValue() const;

private:
    std::stack<float> mStack;
};