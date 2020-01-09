#pragma once

#include "../slr_parser/Rules.h"
#include "../ast/IAST.h"
#include <stack>
#include <map>
#include <memory>

class ASTBuilder
{
public:
    explicit ASTBuilder(std::ostream & debug);

    std::unique_ptr<AST::IAST> build(std::deque<Token::Token> & input);

private:
    SLR::Rules::Table getRules();
    std::unique_ptr<AST::IAST> getAST();

private:
    std::stack<std::unique_ptr<AST::IAST>> mStack;
    std::stack<std::map<std::string, AST::ValueType>> mVariables;
    std::ostream & mDebug;
};