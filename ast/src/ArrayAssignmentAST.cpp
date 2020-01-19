#include "../ArrayAssignmentAST.h"

AST::ArrayAssignmentAST::ArrayAssignmentAST(Token::Token id, std::unique_ptr<IAST> && index, std::unique_ptr<IAST> && val)
        : mId(std::move(id))
        , mIndex(std::move(index))
        , mValue(std::move(val))
{
}

AST::ArrayAssignmentAST::ArrayAssignmentAST(Token::Token id, std::unique_ptr <IAST> &&val)
    : mId(std::move(id))
    , mValue(std::move(val))
{
}

void AST::ArrayAssignmentAST::accept(AST::IASTVisitor &v) const
{
    v.visit(*this);
}

AST::ValueType AST::ArrayAssignmentAST::getResultType() const
{
    return mValue->getResultType();
}

std::string AST::ArrayAssignmentAST::getId() const
{
    return mId.value;
}

void AST::ArrayAssignmentAST::acceptValue(AST::IASTVisitor &v) const
{
    mValue->accept(v);
}

void AST::ArrayAssignmentAST::acceptIndex(AST::IASTVisitor &v) const
{
    mIndex->accept(v);
}

bool AST::ArrayAssignmentAST::hasIndex() const
{
    return mIndex != nullptr;
}
