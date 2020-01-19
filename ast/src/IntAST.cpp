#include "ast/IntAST.h"

using namespace AST;

IntAST::IntAST(Token::Token const& t)
    : mT(t)
{
    if (mT.type != Token::IntLiteral)
    {
        throw std::logic_error("invalid type, when creating int ast: " + Token::tokenDescription(mT));
    }
}

int IntAST::getValue() const
{
    return stoi(mT.value);
}

void IntAST::accept(IASTVisitor & v) const
{
    v.visit(*this);
}

ValueType IntAST::getResultType() const
{
    return ValueType::Int;
}
