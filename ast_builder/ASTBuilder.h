#pragma once

#include "../parser/Rules.h"
#include "../ast/IAST.h"
#include <stack>
#include <memory>

class ASTBuilder
{
public:
    Rules::Table getRules();

    std::unique_ptr<IAST> getAST();

private:
    std::stack<std::unique_ptr<IAST>> mStack;
};