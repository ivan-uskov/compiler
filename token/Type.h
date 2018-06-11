#pragma once

#include <regex>
#include <string>

namespace Token
{
    using Type = size_t;
    static const Type Root = 0x100u;
    static const Type Literal = 0x200u;
    static const Type NonLiteral = 0x300u;

    static const Type SumExpression = Literal | 0x01u;
    static const Type MulExpression = Literal | 0x02u;
    static const Type ValExpression = Literal | 0x03u;

    static const Type Identifier = NonLiteral | 0x01u;                 // ([a-z]+[a-z0-9]*)
    static const Type StringLiteral = NonLiteral | 0x02u;              // (\'[^\']*\')
    static const Type Number = NonLiteral | 0x03u;                     // ([0-9]+)

    static const Type String = NonLiteral | 0x04u;                     // string
    static const Type Return = NonLiteral | 0x05u;                     // return
    static const Type For = NonLiteral | 0x06u;                        // for
    static const Type Int = NonLiteral | 0x07u;                        // int
    static const Type If = NonLiteral | 0x08u;                         // if

    static const Type And = NonLiteral | 0x09u;                        // &&
    static const Type Or = NonLiteral | 0x0Au;                         // ||
    static const Type Equals = NonLiteral | 0x0Bu;                     // ==

    static const Type Comma = NonLiteral | 0x0Cu;                      // ,
    static const Type Semicolon = NonLiteral | 0x0Du;                  // ;
    static const Type Assignment = NonLiteral | 0x0Eu;                 // =
    static const Type Less = NonLiteral | 0x0Fu;                       // <
    static const Type More = NonLiteral | 0x10u;                       // >
    static const Type OpenBrace = NonLiteral | 0x11u;                 // {
    static const Type CloseBrace = NonLiteral | 0x12u;                // }
    static const Type OpenParenthesis = NonLiteral | 0x13u;           // (
    static const Type CloseParenthesis = NonLiteral | 0x14u;          // )
    static const Type Not = NonLiteral | 0x15u;                        // !
    static const Type Plus = NonLiteral | 0x16u;                       // +
    static const Type Minus = NonLiteral | 0x17u;                      // -
    static const Type Mult = NonLiteral | 0x18u;                       // *
    static const Type Div = NonLiteral | 0x19u;                        // /
    static const Type Mod = NonLiteral | 0x20u;                        // %

    static const Type End = NonLiteral | 0x21u;                        // ^Z
    static const Type Invalid = NonLiteral | 0x22u;

    bool isLiteral(Type item);

    std::string tokenTypeToString(Type const & type);
}