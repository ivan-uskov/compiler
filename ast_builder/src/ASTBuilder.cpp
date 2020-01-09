#include <stdexcept>
#include "ast/NumberAST.h"
#include "ast/BinaryOperatorAST.h"
#include "ast/ExpressionPairAST.h"
#include "ast/VariableDeclarationAST.h"
#include "ast_builder/ASTBuilder.h"
#include "slr_parser/Parser.h"

using namespace SLR;
using namespace AST;

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
    Rules::Action getUnaryMinusASTReducer(T & stack)
    {
        return [&stack](auto const& tokens) {
            if (stack.size() < 1)
            {
                throw std::logic_error("too small stack for unary minus");
            }

            auto left = std::make_unique<NumberAST>(Token::Token{Token::NumberLiteral, "0"});

            auto right = std::move(stack.top());
            stack.pop();

            stack.emplace(new BinaryOperatorAST(std::move(left), std::move(right), BinaryOperatorAST::Type::Sub));
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

    template<typename T>
    Rules::Action getExpressionListReducer(T & stack)
    {
        return [&stack](auto const& tokens) {
            if (stack.size() < 2)
            {
                throw std::logic_error("too small stack for expression list");
            }

            auto right = std::move(stack.top());
            stack.pop();
            auto left = std::move(stack.top());
            stack.pop();

            stack.emplace(new ExpressionPairAST(std::move(left), std::move(right)));
        };
    }

    template<typename T>
    Rules::Action getVariableDeclarationASTReducer(T & stack, AST::ValueType v)
    {
        return [&stack, v](auto const& tokens) {
            if (tokens.size() != 2)
            {
                throw std::logic_error("invalid tokens for reduce vardecl");
            }

            stack.emplace(new VariableDeclarationAST(v, tokens[0])); //TODO: check right value
        };
    }
}

Rules::Table ASTBuilder::getRules()
{
    return  {
            {Token::Root,          {Token::StatementList}, getRootReducer(mStack)},
            {Token::StatementList, {Token::Statement, Token::Semicolon,  Token::StatementList}, getExpressionListReducer(mStack)},
            {Token::StatementList, {Token::Statement}},
            {Token::Statement,     {Token::Number, Token::Id}, getVariableDeclarationASTReducer(mStack, AST::ValueType::Number)},
            {Token::Statement,     {Token::String, Token::Id}, getVariableDeclarationASTReducer(mStack, AST::ValueType::String)},
            {Token::Statement,     {Token::Expression}},
            {Token::Expression,    {Token::Expression, Token::Plus, Token::Expression1}, getBinaryOperatorASTReducer(mStack, BinaryOperatorAST::Type::Sum)},
            {Token::Expression,    {Token::Expression, Token::Minus, Token::Expression1}, getBinaryOperatorASTReducer(mStack, BinaryOperatorAST::Type::Sub)},
            {Token::Expression,    {Token::Expression1}},
            {Token::Expression1,   {Token::Expression1, Token::Mult, Token::Expression2}, getBinaryOperatorASTReducer(mStack, BinaryOperatorAST::Type::Mul)},
            {Token::Expression1,   {Token::Expression1, Token::Div, Token::Expression2}, getBinaryOperatorASTReducer(mStack, BinaryOperatorAST::Type::Div)},
            {Token::Expression1,   {Token::Expression2}},
            {Token::Expression2,   {Token::OpenParenthesis, Token::Expression, Token::CloseParenthesis}},
            {Token::Expression2,   {Token::Minus, Token::Expression2}, getUnaryMinusASTReducer(mStack)},
            {Token::Expression2,   {Token::NumberLiteral}, getNumberASTReducer(mStack)}
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