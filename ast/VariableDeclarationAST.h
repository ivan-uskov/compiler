#pragma once

#include "IAST.h"

namespace AST
{
    class VariableDeclarationAST : public IAST
    {
    public:
        explicit VariableDeclarationAST(ValueType v, Token::Token identifier);

        void accept(IASTVisitor &v) const override;

        ValueType getResultType() const override;

        std::string getId() const;

    private:
        ValueType mValueType;
        Token::Token mToken;
    };
}