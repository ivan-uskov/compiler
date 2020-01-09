#include "token/Type.h"

using namespace std;

namespace Token
{
    bool isLiteral(Type item)
    {
        return (item & 0xF00u) == NonTerminal;
    }

    string tokenTypeToString(Token::Type const& type)
    {
        switch (type)
        {
            case Token::Root:             return "Root";

            case Token::Statement:        return "Stmt";
            case Token::StatementList:    return "StmtList";
            case Token::Expression:       return "Expr";
            case Token::Expression1:      return "Expr1";
            case Token::Expression2:      return "Expr2";
            case Token::Expression3:      return "Expr3";
            case Token::VariableDeclaration: return "VarDecl";

            case Token::Id:               return "id";
            case Token::StringLiteral:    return "stringLiteral";
            case Token::NumberLiteral:    return "numberLiteral";

            case Token::String:           return "string";
            case Token::Return:           return "return";
            case Token::For:              return "for";
            case Token::Number:           return "number";
            case Token::If:               return "if";

            case Token::And:              return "&&";
            case Token::Or:               return "||";
            case Token::DoubleEquals:           return "==";

            case Token::Comma:            return ",";
            case Token::Semicolon:        return ";";
            case Token::Equals:       return "=";
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