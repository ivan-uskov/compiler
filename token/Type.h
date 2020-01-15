#pragma once

#include <regex>
#include <string>

namespace Token
{
    using Type = size_t;
    static const Type Root = 0x1000u;
    static const Type NonTerminal = 0x2000u;
    static const Type Terminal = 0x3000u;

    static const Type Statement = NonTerminal | 0x100u;
    static const Type StatementList = NonTerminal | 0x101u;
    static const Type StatementListIf = NonTerminal | 0x102u;
    static const Type StatementListWhile = NonTerminal | 0x103u;
    static const Type Expression = NonTerminal | 0x200u;
    static const Type Expression1 = NonTerminal | 0x201u;
    static const Type Expression2 = NonTerminal | 0x202u;
    static const Type BoolExpression = NonTerminal | 0x203u;
    static const Type BoolExpressionIf = NonTerminal | 0x204u;
    static const Type BoolExpressionWhile = NonTerminal | 0x205u;
    static const Type VariableDeclaration = NonTerminal | 0x300u;

    static const Type Id = Terminal | 0x100u;                         // ([a-z]+[a-z0-9]*)

    static const Type StringLiteral = Terminal | 0x200u;              // (\'[^\']*\')
    static const Type NumberLiteral = Terminal | 0x201u;                     // ([0-9]+)

    static const Type String = Terminal | 0x301u;                     // string
    static const Type Return = Terminal | 0x302u;                     // return
    static const Type While = Terminal | 0x303u;                        // for
    static const Type Number = Terminal | 0x304u;                     // number
    static const Type If = Terminal | 0x305u;                         // if

    static const Type And = Terminal | 0x400u;                        // &&
    static const Type Or = Terminal | 0x401u;                         // ||
    static const Type DoubleEquals = Terminal | 0x402u;               // ==

    static const Type Comma = Terminal | 0x403u;                      // ,
    static const Type Semicolon = Terminal | 0x404u;                  // ;
    static const Type Equals = Terminal | 0x405u;                     // =
    static const Type Less = Terminal | 0x406u;                       // <
    static const Type More = Terminal | 0x407u;                       // >
    static const Type OpenBrace = Terminal | 0x408u;                 // {
    static const Type CloseBrace = Terminal | 0x409u;                // }
    static const Type OpenParenthesis = Terminal | 0x40Au;           // (
    static const Type CloseParenthesis = Terminal | 0x40Bu;          // )
    static const Type Not = Terminal | 0x40Cu;                        // !
    static const Type Plus = Terminal | 0x40Du;                       // +
    static const Type Minus = Terminal | 0x40Eu;                      // -
    static const Type Mult = Terminal | 0x40Fu;                       // *
    static const Type Div = Terminal | 0x410u;                        // /
    static const Type Mod = Terminal | 0x411u;                        // %

    static const Type End = Terminal | 0x500u;                        // ^Z
    static const Type Invalid = Terminal | 0x600u;

    bool isLiteral(Type item);

    std::string tokenTypeToString(Type const & type);
}