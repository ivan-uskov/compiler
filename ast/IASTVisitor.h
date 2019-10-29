#pragma once

class BinaryOperatorAST;
class NumberAST;

class IASTVisitor
{
public:
    virtual ~IASTVisitor() = default;
    virtual void visit(BinaryOperatorAST const& op) = 0;
    virtual void visit(NumberAST const& op) = 0;
};