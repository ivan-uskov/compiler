#include "../VariableDeclarationAST.h"

#include <utility>

AST::VariableDeclarationAST::VariableDeclarationAST(AST::ValueType v, Token::Token identifier)
    : mValueType(v)
    , mToken(std::move(identifier))
{
}

void AST::VariableDeclarationAST::accept(AST::IASTVisitor &v) const
{
    v.visit(*this);
}

AST::ValueType AST::VariableDeclarationAST::getResultType() const
{
    return mValueType;
}

std::string AST::VariableDeclarationAST::getId() const
{
    return mToken.value;
}
