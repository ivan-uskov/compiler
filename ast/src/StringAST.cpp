#include "../StringAST.h"

using namespace AST;

StringAST::StringAST(Token::Token const& t)
        : mT(t)
{
    if (mT.type != Token::StringLiteral)
    {
        throw std::logic_error("invalid type, when creating string ast: " + Token::tokenDescription(mT));
    }
}

std::string StringAST::getValue() const
{
    return mT.value;
}

void StringAST::accept(IASTVisitor & v) const
{
    v.visit(*this);
}

ValueType StringAST::getResultType() const
{
    return ValueType::String;
}
