#pragma once

#include "IAST.h"

namespace AST
{
    class VariableAccessAST : public IAST
    {
    public:
        explicit VariableAccessAST(Token::Token id, ValueType t);

        void accept(IASTVisitor &v) const override;

        ValueType getResultType() const override;

        std::string getId() const;

    private:
        Token::Token mId;
        ValueType mType;
    };
}