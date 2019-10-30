#pragma once

#include "../slr_parser/Rules.h"
#include "../ast/IAST.h"
#include <stack>
#include <memory>

class ASTBuilder
{
public:
    explicit ASTBuilder(std::ostream & debug);

    std::unique_ptr<IAST> build(std::deque<Token::Token> & input);

private:
    SLR::Rules::Table getRules();
    std::unique_ptr<IAST> getAST();

private:
    std::stack<std::unique_ptr<IAST>> mStack;
    std::ostream & mDebug;
};