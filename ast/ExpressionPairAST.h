#pragma once

#include "IAST.h"

namespace AST
{
    class ExpressionPairAST : public IAST
    {
    public:
        explicit ExpressionPairAST(std::unique_ptr<IAST> && left, std::unique_ptr<IAST> && right);
        void accept(IASTVisitor & v) const override;
        void acceptLeft(IASTVisitor & v) const;
        void acceptRight(IASTVisitor & v) const;

    private:
        std::unique_ptr<IAST> mLeft;
        std::unique_ptr<IAST> mRight;
    };
}