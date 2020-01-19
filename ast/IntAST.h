#pragma once

#include <stdexcept>

#include "IAST.h"

namespace AST
{
    class IntAST : public IAST
    {
    public:
        explicit IntAST(Token::Token const& t);

        void accept(IASTVisitor & v) const override;

        int getValue() const;

        ValueType getResultType() const override;

    private:
        Token::Token mT;
    };
}