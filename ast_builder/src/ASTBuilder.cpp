#include <stdexcept>
#include "ast/NumberAST.h"
#include "ast/BinaryOperatorAST.h"
#include "ast_builder/ASTBuilder.h"

Rules::Table ASTBuilder::getRules()
{
    return  {
            {Token::Root,          {Token::Expression}, [&](auto const& tokens) {
                if (mStack.size() != 1)
                {
                    throw std::logic_error("invalid stack size in the end");
                }
            }},
            {Token::Expression, {Token::Expression,   Token::Plus,          Token::Expression}, [&](auto const& tokens) {
                if (mStack.size() < 2)
                {
                    throw std::logic_error("too small stack for binary operator");
                }

                auto right = std::move(mStack.top());
                mStack.pop();
                auto left = std::move(mStack.top());
                mStack.pop();

                mStack.emplace(new BinaryOperatorAST(std::move(left), std::move(right), BinaryOperatorAST::Type::Sum));
            }},
            {Token::Expression, {Token::Expression,   Token::Mult,          Token::Expression}, [&](auto const& tokens) {
                 if (mStack.size() < 2)
                 {
                     throw std::logic_error("too small stack for binary operator");
                 }

                 auto right = std::move(mStack.top());
                 mStack.pop();
                 auto left = std::move(mStack.top());
                 mStack.pop();

                 mStack.emplace(new BinaryOperatorAST(std::move(left), std::move(right), BinaryOperatorAST::Type::Mul));
            }},
            {Token::Expression, {Token::OpenParenthesis, Token::Expression, Token::CloseParenthesis}},
            {Token::Expression, {Token::Number}, [&](auto const& tokens){
                if (tokens.size() != 1)
                {
                    throw std::logic_error("invalid tokens for reduce number");
                }

                mStack.emplace(new NumberAST(tokens[0]));
            }}
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