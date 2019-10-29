#include "../NumberAST.h"

NumberAST::NumberAST(Token::Token const& t)
    : mT(t)
{
    if (mT.type != Token::Number)
    {
        throw std::logic_error("invalid type, when creating number ast: " + Token::tokenTypeToString(mT.type));
    }
}

float NumberAST::getValue() const
{
    return stof(mT.value);
}

void NumberAST::accept(IASTVisitor & v) const
{
    v.visit(*this);
}