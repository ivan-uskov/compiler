#pragma once

#include "IAST.h"

namespace AST
{
    class FunctionCallAST : public IAST
    {
    public:
        explicit FunctionCallAST(std::unique_ptr<IAST> && arg);

        void accept(IASTVisitor &v) const override;
        void acceptArgument(IASTVisitor &v) const;

        ValueType getResultType() const override;

    private:
        std::unique_ptr<IAST> mArg;
    };
}