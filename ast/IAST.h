#pragma once

#include "token/Token.h"
#include "token/Type.h"

#include "IASTVisitor.h"
#include "ValueType.h"

namespace AST
{
    class IAST
    {
    public:
        virtual ~IAST() = default;
        virtual void accept(IASTVisitor & v) const = 0;
        virtual ValueType getResultType() const = 0;
    };
}