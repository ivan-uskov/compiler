#pragma once
#include <vector>

class Rules
{
public:
    enum Item
    {
        None = 0u,
        Root = 0x100u,
        Literal = 0x200u,
        NonLiteral = 0x300u,

        Number = NonLiteral | 0x01u,
        Plus = NonLiteral | 0x02u,
        Mul = NonLiteral | 0x03u,
        OPEN_PARENTHESIS = NonLiteral | 0x04u,           // (
        CLOSE_PARENTHESIS = NonLiteral | 0x05u,          // )
        Eof = NonLiteral | 0x06u,

        SumExpression = Literal | 0x01u,
        MulExpression = Literal | 0x02u,
        ValExpression = Literal | 0x03u
    };

    static bool isLiteral(Item item)
    {
        return (item & 0xF00u) == Literal;
    }

    using Table = std::vector<std::pair<Item, std::vector<Item>>>;

    static Table rules = {
        {Root, {SumExpression}},
        {SumExpression, {SumExpression, Plus, MulExpression}},
        {SumExpression, {MulExpression}},
        {MulExpression, {MulExpression, Mul, ValExpression}},
        {MulExpression, {ValExpression}},
        {ValExpression, {OPEN_PARENTHESIS, SumExpression, CLOSE_PARENTHESIS}},
        {ValExpression, {Number}}
    };
};
