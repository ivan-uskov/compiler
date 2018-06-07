#pragma once
#include <regex>
#include <string>

namespace Lexer
{
    //WARNNG Types should stay from large pattern size to least
    enum class TokenType
    {
        IDENTIFICATOR,              // ([a-z]+[a-z0-9]*)
        STRING_LITERAL,             // (\'[^\']*\')
        NUMBER,                     // ([0-9]+)

        STRING,                     // string
        RETURN,                     // return
        FOR,                        // for
        INT,                        // int
        IF,                         // if

        //2
        AND,                        // &&
        OR,                         // ||
        EQUALS,                     // ==

        //1
        COMMA,                      // ,
        SEMICOLON,                  // ;
        ASSIGNMENT,                 // =
        LESS,                       // <
        MORE,                       // >
        OPEN_BRACE,                 // {
        CLOSE_BRACE,                // }
        OPEN_PARENTHESIS,           // (
        CLOSE_PARENTHESIS,          // )
        NOT,                        // !
        PLUS,                       // +
        MINUS,                      // -
        MULT,                       // *
        DIV,                        // /
        MOD,                        // %

        END,                        // ^Z
        Invalid
    };

    std::string tokenTypeToString(TokenType const& type);
}