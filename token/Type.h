#pragma once

#include <regex>
#include <string>

namespace Token
{
    using Type = size_t;
    static const Type Root = 0x100u;
    static const Type NonTerminal = 0x200u;
    static const Type Terminal = 0x300u;

    static const Type SumExpression = NonTerminal | 0x01u;
    static const Type MulExpression = NonTerminal | 0x02u;
    static const Type Expression = NonTerminal | 0x03u;

    // testing
    static const Type S = NonTerminal | 0x04u;
    static const Type A = NonTerminal | 0x05u;
    static const Type B = NonTerminal | 0x06u;
    static const Type C = NonTerminal | 0x07u;
    static const Type E = NonTerminal | 0x08u;
    static const Type X = NonTerminal | 0x09u;
    static const Type Y = NonTerminal | 0x0Au;

    static const Type a = Terminal | 0x1Du;
    static const Type b = Terminal | 0x1Eu;
    static const Type c = Terminal | 0x1Fu;
    static const Type i = Terminal | 0x20u;
    static const Type Else = Terminal | 0x21u;
    static const Type e = Terminal | 0x22u;
    static const Type login = Terminal | 0x23u;
    static const Type d = Terminal | 0x24u;
    static const Type s = Terminal | 0x25u;
    static const Type While = Terminal | 0x26u;
    static const Type Do = Terminal | 0x27u;
    // testing end

    static const Type Id = Terminal | 0x01u;                         // ([a-z]+[a-z0-9]*)
    static const Type StringLiteral = Terminal | 0x02u;              // (\'[^\']*\')
    static const Type Number = Terminal | 0x03u;                     // ([0-9]+)

    static const Type String = Terminal | 0x04u;                     // string
    static const Type Return = Terminal | 0x05u;                     // return
    static const Type For = Terminal | 0x06u;                        // for
    static const Type Int = Terminal | 0x07u;                        // int
    static const Type If = Terminal | 0x08u;                         // if

    static const Type And = Terminal | 0x09u;                        // &&
    static const Type Or = Terminal | 0x0Au;                         // ||
    static const Type Equals = Terminal | 0x0Bu;                     // ==

    static const Type Comma = Terminal | 0x0Cu;                      // ,
    static const Type Semicolon = Terminal | 0x0Du;                  // ;
    static const Type Assignment = Terminal | 0x0Eu;                 // =
    static const Type Less = Terminal | 0x0Fu;                       // <
    static const Type More = Terminal | 0x10u;                       // >
    static const Type OpenBrace = Terminal | 0x11u;                 // {
    static const Type CloseBrace = Terminal | 0x12u;                // }
    static const Type OpenParenthesis = Terminal | 0x13u;           // (
    static const Type CloseParenthesis = Terminal | 0x14u;          // )
    static const Type Not = Terminal | 0x15u;                        // !
    static const Type Plus = Terminal | 0x16u;                       // +
    static const Type Minus = Terminal | 0x17u;                      // -
    static const Type Mult = Terminal | 0x18u;                       // *
    static const Type Div = Terminal | 0x19u;                        // /
    static const Type Mod = Terminal | 0x1Au;                        // %

    static const Type End = Terminal | 0x1Bu;                        // ^Z
    static const Type Invalid = Terminal | 0x1Cu;

    bool isLiteral(Type item);

    std::string tokenTypeToString(Type const & type);
}