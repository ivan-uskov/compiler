#pragma once

#include <regex>
#include <string>

namespace Token
{
    enum Type
    {
        None = 0u,
        Root = 0x100u,
        Literal = 0x200u,
        NonLiteral = 0x300u,

        SumExpression = Literal | 0x01u,
        MulExpression = Literal | 0x02u,
        ValExpression = Literal | 0x03u,

        Identifier = NonLiteral | 0x01u,                 // ([a-z]+[a-z0-9]*)
        StringLiteral = NonLiteral | 0x02u,              // (\'[^\']*\')
        Number = NonLiteral | 0x03u,                     // ([0-9]+)

        String = NonLiteral | 0x04u,                     // string
        Return = NonLiteral | 0x05u,                     // return
        For = NonLiteral | 0x06u,                        // for
        Int = NonLiteral | 0x07u,                        // int
        If = NonLiteral | 0x08u,                         // if

        //2
        And = NonLiteral | 0x09u,                        // &&
        Or = NonLiteral | 0x0Au,                         // ||
        Equals = NonLiteral | 0x0Bu,                     // ==

        //1
        Comma = NonLiteral | 0x0Cu,                      // ,
        Semicolon = NonLiteral | 0x0Du,                  // ;
        Assignment = NonLiteral | 0x0Eu,                 // =
        Less = NonLiteral | 0x0Fu,                       // <
        More = NonLiteral | 0x10u,                       // >
        OpenBrace = NonLiteral | 0x11u,                 // {
        CloseBrace = NonLiteral | 0x12u,                // }
        OpenParenthesis = NonLiteral | 0x13u,           // (
        CloseParenthesis = NonLiteral | 0x14u,          // )
        Not = NonLiteral | 0x15u,                        // !
        Plus = NonLiteral | 0x16u,                       // +
        Minus = NonLiteral | 0x17u,                      // -
        Mult = NonLiteral | 0x18u,                       // *
        Div = NonLiteral | 0x19u,                        // /
        Mod = NonLiteral | 0x20u,                        // %

        End = NonLiteral | 0x21u,                        // ^Z
        Invalid = NonLiteral | 0x22u
    };

    std::string tokenTypeToString(Type const & type);
}