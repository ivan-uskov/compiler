#include "../BoolAST.h"

using namespace AST;

BoolAST::BoolAST(Token::Token const& t)
        : mT(t)
{
    if (mT.type != Token::BoolLiteral)
    {
        throw std::logic_error("invalid type, when creating int ast: " + Token::tokenDescription(mT));
    }
}

bool BoolAST::getValue() const
{
    return mT.value == "true";
}

void BoolAST::accept(IASTVisitor & v) const
{
    v.visit(*this);
}

ValueType BoolAST::getResultType() const
{
    return ValueType::Bool;
}
