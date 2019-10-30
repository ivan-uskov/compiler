#pragma once

#include "token/Token.h"
#include "token/Type.h"

#include "IASTVisitor.h"

namespace AST
{
    class IAST
    {
    public:
        virtual ~IAST() = default;
        virtual void accept(IASTVisitor & v) const = 0;
    };
}