#include "ast/DoubleAST.h"

using namespace AST;

DoubleAST::DoubleAST(Token::Token const& t)
        : mT(t)
{
    if (mT.type != Token::DoubleLiteral)
    {
        throw std::logic_error("invalid type, when creating double ast: " + Token::tokenDescription(mT));
    }
}

double DoubleAST::getValue() const
{
    return stod(mT.value);
}

void DoubleAST::accept(IASTVisitor & v) const
{
    v.visit(*this);
}

ValueType DoubleAST::getResultType() const
{
    return ValueType::Double;
}
