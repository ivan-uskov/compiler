#include "lexer/TokenType.h"

using namespace std;

namespace Lexer
{
    string tokenTypeToString(TokenType const& type)
    {
        switch (type)
        {
        case TokenType::IDENTIFICATOR:
            return "ident";
        case TokenType::STRING_LITERAL:
            return "'<some lit>'";
        case TokenType::NUMBER:
            return "number";

        case TokenType::STRING:
            return "string";
        case TokenType::RETURN:
            return "return";
        case TokenType::FOR:
            return "for";
        case TokenType::INT:
            return "int";
        case TokenType::IF:
            return "if";

        case TokenType::AND:
            return "&&";
        case TokenType::OR:
            return "||";
        case TokenType::EQUALS:
            return "==";

        case TokenType::COMMA:
            return ",";
        case TokenType::SEMICOLON:
            return ";";
        case TokenType::ASSIGNMENT:
            return "=";
        case TokenType::LESS:
            return "<";
        case TokenType::MORE:
            return ">";
        case TokenType::OPEN_BRACE:
            return "{";
        case TokenType::CLOSE_BRACE:
            return "}";
        case TokenType::OPEN_PARENTHESIS:
            return "(";
        case TokenType::CLOSE_PARENTHESIS:
            return ")";
        case TokenType::NOT:
            return "!";
        case TokenType::PLUS:
            return "+";
        case TokenType::MINUS:
            return "-";
        case TokenType::MULT:
            return "*";
        case TokenType::DIV:
            return "/";
        case TokenType::MOD:
            return "%";

        case TokenType::END:
            return "EOF";
        default:
            return "Unexpected token type";
        }
    }
}