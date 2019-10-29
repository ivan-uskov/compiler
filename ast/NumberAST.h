#pragma once

#include <stdexcept>

#include "IAST.h"

class NumberAST : public IAST
{
public:
    explicit NumberAST(Token::Token const& t);

    void accept(IASTVisitor & v) const override;

    float getValue() const;
private:
    Token::Token mT;
};