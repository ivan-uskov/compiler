#pragma once
#include <ostream>

#include "IASTVisitor.h"

class ASTView : public IASTVisitor
{
public:
    ASTView(std::ostream & out);

    void visit(BinaryOperatorAST const& op);
    void visit(NumberAST const& op);
private:
    std::ostream & mOut;
};