#include "token/Type.h"

using namespace std;

namespace Token
{
    bool isLiteral(Type item)
    {
        return (item & 0xF00u) == Literal;
    }

    string tokenTypeToString(Token::Type const& type)
    {
        switch (type)
        {
            case Token::Root:             return "Root";

            case Token::SumExpression:    return "SE";
            case Token::MulExpression:    return "ME";
            case Token::ValExpression:    return "VE";

            case Token::Identifier:       return "Identifier";
            case Token::StringLiteral:    return "StringLiteral";
            case Token::Number:           return "Number";

            case Token::String:           return "String";
            case Token::Return:           return "Return";
            case Token::For:              return "For";
            case Token::Int:              return "Int";
            case Token::If:               return "If";

            case Token::And:              return "&&";
            case Token::Or:               return "||";
            case Token::Equals:           return "==";

            case Token::Comma:            return ",";
            case Token::Semicolon:        return ";";
            case Token::Assignment:       return "=";
            case Token::Less:             return "<";
            case Token::More:             return ">";
            case Token::OpenBrace:        return "{";
            case Token::CloseBrace:       return "}";
            case Token::OpenParenthesis:  return "(";
            case Token::CloseParenthesis: return ")";
            case Token::Not:              return "!";
            case Token::Plus:             return "+";
            case Token::Minus:            return "-";
            case Token::Mult:             return "*";
            case Token::Div:              return "/";
            case Token::Mod:              return "%";

            case Token::End:              return "End";
            default:                            return "Unexpected token type";
        }
    }
}