#pragma once

#include <regex>
#include <string>

namespace Token
{
    using Type = size_t;
    static const Type Root = 0x100u;
    static const Type NonTerminal = 0x200u;
    static const Type Terminal = 0x300u;

    static const Type Statement = NonTerminal | 0x01u;
    static const Type StatementList = NonTerminal | 0x02u;
    static const Type Expression = NonTerminal | 0x03u;
    static const Type Expression1 = NonTerminal | 0x031;
    static const Type Expression2 = NonTerminal | 0x032;
    static const Type Expression3 = NonTerminal | 0x033;
    static const Type VariableDeclaration = NonTerminal | 0x04u;

    static const Type Id = Terminal | 0x01u;                         // ([a-z]+[a-z0-9]*)
    static const Type StringLiteral = Terminal | 0x02u;              // (\'[^\']*\')
    static const Type NumberLiteral = Terminal | 0x03u;                     // ([0-9]+)

    static const Type String = Terminal | 0x04u;                     // string
    static const Type Return = Terminal | 0x05u;                     // return
    static const Type For = Terminal | 0x06u;                        // for
    static const Type Number = Terminal | 0x07u;                     // number
    static const Type If = Terminal | 0x08u;                         // if

    static const Type And = Terminal | 0x09u;                        // &&
    static const Type Or = Terminal | 0x0Au;                         // ||
    static const Type DoubleEquals = Terminal | 0x0Bu;               // ==

    static const Type Comma = Terminal | 0x0Cu;                      // ,
    static const Type Semicolon = Terminal | 0x0Du;                  // ;
    static const Type Equals = Terminal | 0x0Eu;                     // =
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