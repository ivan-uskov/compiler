#pragma once
#include <vector>
#include "token/Type.h"

class Rules {
public:
    using Item = Token::Type;

    static bool isLiteral(Item item) {
        return (item & 0xF00u) == Item::Literal;
    }

    using Table = std::vector<std::pair<Item, std::vector<Item>>>;

    static Table get()
    {
        return {
                {Item::Root,          {Item::SumExpression}},
                {Item::SumExpression, {Item::SumExpression,    Item::Plus,          Item::MulExpression}},
                {Item::SumExpression, {Item::MulExpression}},
                {Item::MulExpression, {Item::MulExpression,    Item::Mult,           Item::ValExpression}},
                {Item::MulExpression, {Item::ValExpression}},
                {Item::ValExpression, {Item::OpenParenthesis, Item::SumExpression, Item::CloseParenthesis}},
                {Item::ValExpression, {Item::Number}}
        };
    }
};
