#include "../NumberAST.h"

using namespace AST;

NumberAST::NumberAST(Token::Token const& t)
    : mT(t)
{
    if (mT.type != Token::Number)
    {
        throw std::logic_error("invalid type, when creating number ast: " + Token::tokenTypeToString(mT.type));
    }
}

double NumberAST::getValue() const
{
    return stod(mT.value);
}

void NumberAST::accept(IASTVisitor & v) const
{
    v.visit(*this);
}