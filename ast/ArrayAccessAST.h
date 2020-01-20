#pragma once

#include "IAST.h"

namespace AST
{
    class ArrayAccessAST : public IAST
    {
    public:
        explicit ArrayAccessAST(Token::Token id, std::unique_ptr<IAST> && index, ValueType t);

        void accept(IASTVisitor &v) const override;
        void acceptIndex(IASTVisitor &v) const;

        ValueType getResultType() const override;

        std::string getId() const;

    private:
        Token::Token mId;
        ValueType mType;
        std::unique_ptr<IAST> mIndex;
    };
}