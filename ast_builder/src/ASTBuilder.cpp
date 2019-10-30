#include <stdexcept>
#include "ast/NumberAST.h"
#include "ast/BinaryOperatorAST.h"
#include "ast_builder/ASTBuilder.h"
#include "slr_parser/Parser.h"

using namespace SLR;

namespace
{
    template<typename T>
    Rules::Action getBinaryOperatorASTReducer(T & stack, BinaryOperatorAST::Type t)
    {
        return [&stack, t](auto const& tokens) {
            if (stack.size() < 2)
            {
                throw std::logic_error("too small stack for binary operator");
            }

            auto right = std::move(stack.top());
            stack.pop();
            auto left = std::move(stack.top());
            stack.pop();

            stack.emplace(new BinaryOperatorAST(std::move(left), std::move(right), t));
        };
    }

    template<typename T>
    Rules::Action getNumberASTReducer(T & stack)
    {
        return [&stack](auto const& tokens) {
            if (tokens.size() != 1)
            {
                throw std::logic_error("invalid tokens for reduce number");
            }

            stack.emplace(new NumberAST(tokens[0]));
        };
    }

    template<typename T>
    Rules::Action getRootReducer(T const& stack)
    {
        return [&stack](auto const& tokens) {
            if (stack.size() != 1)
            {
                throw std::logic_error("invalid stack size in the end");
            }
        };
    }
}

Rules::Table ASTBuilder::getRules()
{
    return  {
            {Token::Root,       {Token::Expression}, getRootReducer(mStack)},
            {Token::Expression, {Token::Expression, Token::Plus,  Token::Expression}, getBinaryOperatorASTReducer(mStack, BinaryOperatorAST::Type::Sum)},
            {Token::Expression, {Token::Expression, Token::Minus, Token::Expression}, getBinaryOperatorASTReducer(mStack, BinaryOperatorAST::Type::Sub)},
            {Token::Expression, {Token::Expression, Token::Mult,  Token::Expression}, getBinaryOperatorASTReducer(mStack, BinaryOperatorAST::Type::Mul)},
            {Token::Expression, {Token::Expression, Token::Div,   Token::Expression}, getBinaryOperatorASTReducer(mStack, BinaryOperatorAST::Type::Div)},
            {Token::Expression, {Token::OpenParenthesis, Token::Expression, Token::CloseParenthesis}},
            {Token::Expression, {Token::Number}, getNumberASTReducer(mStack)}
    };
}

std::unique_ptr<IAST> ASTBuilder::getAST()
{
    if (mStack.size() != 1)
    {
        throw std::logic_error("ast not built");
    }

    auto ast = std::move(mStack.top());
    mStack.pop();
    return std::move(ast);
}

std::unique_ptr<IAST> ASTBuilder::build(std::deque<Token::Token> & tokens)
{
    Parser(getRules(), mDebug).parse(tokens);
    return getAST();
}

ASTBuilder::ASTBuilder(std::ostream & debug)
    : mDebug(debug)
{}