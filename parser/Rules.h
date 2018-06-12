#pragma once

#include <vector>
#include "token/Type.h"

class Rules
{
public:
    using Item = Token::Type;
    using Table = std::vector<std::pair<Token::Type, std::vector<Token::Type>>>;

    static Table get()
    {
        return {
                {Token::Root,          {Token::SumExpression}},
                {Token::SumExpression, {Token::SumExpression,   Token::Plus,          Token::MulExpression}},
                {Token::SumExpression, {Token::MulExpression}},
                {Token::MulExpression, {Token::MulExpression,   Token::Mult,          Token::ValExpression}},
                {Token::MulExpression, {Token::ValExpression}},
                {Token::ValExpression, {Token::OpenParenthesis, Token::SumExpression, Token::CloseParenthesis}},
                {Token::ValExpression, {Token::Number}}
        };
    }
};
