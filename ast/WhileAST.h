#pragma once

#include "IAST.h"

namespace AST
{
    class WhileAST : public IAST
    {
    public:
        explicit WhileAST(std::unique_ptr<IAST> && cond, std::unique_ptr<IAST> && stmt);

        void accept(IASTVisitor &v) const override;
        void acceptCond(IASTVisitor &v) const;
        void acceptStmt(IASTVisitor &v) const;

        ValueType getResultType() const override;

    private:
        std::unique_ptr<IAST> mCond;
        std::unique_ptr<IAST> mStmt;
    };
}