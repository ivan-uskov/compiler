#pragma once

#include <stdexcept>

#include "IAST.h"

namespace AST
{
    class NumberAST : public IAST
    {
    public:
        explicit NumberAST(Token::Token const& t);

        void accept(IASTVisitor & v) const override;

        double getValue() const;

        ValueType getResultType() const override;

    private:
        Token::Token mT;
    };
}