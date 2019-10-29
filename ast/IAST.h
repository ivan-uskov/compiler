#pragma once

#include "token/Token.h"
#include "token/Type.h"

#include "IASTVisitor.h"

class IAST
{
public:
    virtual ~IAST() = default;
    virtual void accept(IASTVisitor & v) const = 0;
};