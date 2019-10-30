#pragma once

#include "IAST.h"

class BinaryOperatorAST : public IAST
{
public:
    enum class Type
    {
        Sum,
        Mul,
        Div,
        Sub
    };
    explicit BinaryOperatorAST(std::unique_ptr<IAST> && left, std::unique_ptr<IAST> && right, Type t);
    void accept(IASTVisitor & v) const override;
    void acceptLeft(IASTVisitor & v) const;
    void acceptRight(IASTVisitor & v) const;
    Type getType() const;

private:
    std::unique_ptr<IAST> mLeft;
    std::unique_ptr<IAST> mRight;
    Type mType;
};

std::ostream & operator << (std::ostream & out, BinaryOperatorAST::Type t);