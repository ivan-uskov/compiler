#pragma once

#include <stdexcept>

#include "IAST.h"

namespace AST
{
    class BoolAST : public IAST
    {
    public:
        explicit BoolAST(Token::Token const& t);

        void accept(IASTVisitor & v) const override;

        bool getValue() const;

        ValueType getResultType() const override;

    private:
        Token::Token mT;
    };
}