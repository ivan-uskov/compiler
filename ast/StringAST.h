#pragma once

#include "IAST.h"

namespace AST
{
    class StringAST : public IAST
    {
    public:
        explicit StringAST(Token::Token const& t);

        void accept(IASTVisitor & v) const override;

        std::string getValue() const;

        ValueType getResultType() const override;

    private:
        Token::Token mT;
    };
}