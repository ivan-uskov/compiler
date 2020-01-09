#pragma once

#include "IAST.h"

namespace AST
{
    class AssignmentAST : public IAST
    {
    public:
        explicit AssignmentAST(Token::Token id, std::unique_ptr<IAST> && val);

        void accept(IASTVisitor &v) const override;
        void acceptValue(IASTVisitor & v) const;

        ValueType getResultType() const override;

        std::string getId() const;

    private:
        Token::Token mId;
        std::unique_ptr<IAST> mValue;
    };
}