#include "../AssignmentAST.h"

AST::AssignmentAST::AssignmentAST(Token::Token id, std::unique_ptr<IAST> && val)
    : mId(std::move(id))
    , mValue(std::move(val))
{
}

void AST::AssignmentAST::accept(AST::IASTVisitor &v) const
{
    v.visit(*this);
}

AST::ValueType AST::AssignmentAST::getResultType() const
{
    return mValue->getResultType();
}

std::string AST::AssignmentAST::getId() const
{
    return mId.value;
}

void AST::AssignmentAST::acceptValue(AST::IASTVisitor &v) const
{
    mValue->accept(v);
}
