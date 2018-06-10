#include "token/Type.h"

using namespace std;

namespace Token
{
    string tokenTypeToString(Token::Type const& type)
    {
        switch (type)
        {
            case Token::Type::Identifier:
                return "ident";
            case Token::Type::StringLiteral:
                return "'<some lit>'";
            case Token::Type::Number:
                return "number";

            case Token::Type::String:
                return "string";
            case Token::Type::Return:
                return "return";
            case Token::Type::For:
                return "for";
            case Token::Type::Int:
                return "int";
            case Token::Type::If:
                return "if";

            case Token::Type::And:
                return "&&";
            case Token::Type::Or:
                return "||";
            case Token::Type::Equals:
                return "==";

            case Token::Type::Comma:
                return ",";
            case Token::Type::Semicolon:
                return ";";
            case Token::Type::Assignment:
                return "=";
            case Token::Type::Less:
                return "<";
            case Token::Type::More:
                return ">";
            case Token::Type::OpenBrace:
                return "{";
            case Token::Type::CloseBrace:
                return "}";
            case Token::Type::OpenParenthesis:
                return "(";
            case Token::Type::CloseParenthesis:
                return ")";
            case Token::Type::Not:
                return "!";
            case Token::Type::Plus:
                return "+";
            case Token::Type::Minus:
                return "-";
            case Token::Type::Mult:
                return "*";
            case Token::Type::Div:
                return "/";
            case Token::Type::Mod:
                return "%";

            case Token::Type::End:
                return "EOF";
            default:
                return "Unexpected token type";
        }
    }
}