#include "../ArrayAccessAST.h"

AST::ArrayAccessAST::ArrayAccessAST(Token::Token id, std::unique_ptr<IAST> && index, ValueType t)
        : mId(std::move(id))
        , mType(t)
        , mIndex(std::move(index))
{
}

void AST::ArrayAccessAST::accept(AST::IASTVisitor &v) const
{
    v.visit(*this);
}

AST::ValueType AST::ArrayAccessAST::getResultType() const
{
    return mType;
}

std::string AST::ArrayAccessAST::getId() const
{
    return mId.value;
}

void AST::ArrayAccessAST::acceptIndex(AST::IASTVisitor &v) const
{
    mIndex->accept(v);
}
