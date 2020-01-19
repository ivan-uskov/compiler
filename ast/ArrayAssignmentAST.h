#pragma once

#include "IAST.h"

namespace AST
{
    class ArrayAssignmentAST : public IAST
    {
    public:
        explicit ArrayAssignmentAST(Token::Token id, std::unique_ptr<IAST> && index, std::unique_ptr<IAST> && val);
        explicit ArrayAssignmentAST(Token::Token id, std::unique_ptr<IAST> && val);

        void accept(IASTVisitor &v) const override;
        void acceptValue(IASTVisitor & v) const;
        void acceptIndex(IASTVisitor & v) const;

        ValueType getResultType() const override;

        bool hasIndex() const;
        std::string getId() const;

    private:
        Token::Token mId;
        std::unique_ptr<IAST> mIndex;
        std::unique_ptr<IAST> mValue;
    };
}