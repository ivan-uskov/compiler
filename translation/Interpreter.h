#pragma once

#include <stack>
#include "ast/IASTVisitor.h"

class Interpreter : public IASTVisitor
{
public:
    void visit(BinaryOperatorAST const& op) override;
    void visit(NumberAST const& op) override;

    float getValue() const;

private:
    std::stack<float> mStack;
};