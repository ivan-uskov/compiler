#pragma once

#include "IAST.h"

namespace AST
{
    class FunctionCallAST : public IAST
    {
    public:
        static const std::string PRINT;
        static const std::string PRINTLN;

        static bool isSupportedFunctionName(std::string const& name);
    public:
        explicit FunctionCallAST(std::string name, std::unique_ptr<IAST> && arg);

        void accept(IASTVisitor &v) const override;
        void acceptArgument(IASTVisitor &v) const;

        std::string getName() const;
        ValueType getResultType() const override;

    private:
        std::unique_ptr<IAST> mArg;
        std::string mName;
    };
}