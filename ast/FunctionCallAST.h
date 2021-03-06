#pragma once

#include "IAST.h"

namespace AST
{
    class FunctionCallAST : public IAST
    {
    public:
        static const std::string READ_BOOL;
        static const std::string READ_INT;
        static const std::string READ_DOUBLE;
        static const std::string READ_STRING;
        static const std::string PRINT;
        static const std::string PRINTLN;

        static bool isSupportedFunctionName(std::string const& name);
        static bool isReadFunctionName(std::string const& name);
        static bool isPrintFunctionName(std::string const& name);
    public:
        explicit FunctionCallAST(std::string name, std::unique_ptr<IAST> && arg);
        explicit FunctionCallAST(std::string name);

        void accept(IASTVisitor &v) const override;
        void acceptArgument(IASTVisitor &v) const;

        std::string getName() const;
        ValueType getResultType() const override;
        ValueType getArgumentType() const;

    private:
        std::unique_ptr<IAST> mArg;
        std::string mName;
    };
}