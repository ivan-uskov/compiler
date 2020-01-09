#include "../VariableAccessAST.h"

AST::VariableAccessAST::VariableAccessAST(Token::Token id, ValueType t)
    : mId(std::move(id))
    , mType(t)
{
}

void AST::VariableAccessAST::accept(AST::IASTVisitor &v) const
{
    v.visit(*this);
}

AST::ValueType AST::VariableAccessAST::getResultType() const
{
    return mType;
}

std::string AST::VariableAccessAST::getId() const
{
    return mId.value;
}
