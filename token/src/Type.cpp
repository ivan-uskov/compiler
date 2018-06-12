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

            case Token::SumExpression:    return "S";
            case Token::MulExpression:    return "M";
            case Token::ValExpression:    return "V";

            case Token::Identifier:       return "identifier";
            case Token::StringLiteral:    return "stringLiteral";
            case Token::Number:           return "n";

            case Token::String:           return "string";
            case Token::Return:           return "return";
            case Token::For:              return "for";
            case Token::Int:              return "int";
            case Token::If:               return "if";

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

            case Token::End:              return "end";
            default:                            return "Unexpected token type";
        }
    }
}