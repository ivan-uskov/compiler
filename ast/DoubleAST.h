#pragma once

#include <stdexcept>

#include "IAST.h"

namespace AST
{
    class DoubleAST : public IAST
    {
    public:
        explicit DoubleAST(Token::Token const& t);

        void accept(IASTVisitor & v) const override;

        double getValue() const;

        ValueType getResultType() const override;

    private:
        Token::Token mT;
    };
}