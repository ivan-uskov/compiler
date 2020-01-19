#include "token/Type.h"

using namespace std;

namespace Token
{
    bool isLiteral(Type item)
    {
        return (item & 0xF000u) == NonTerminal;
    }

    string tokenTypeToString(Token::Type const& type)
    {
        switch (type)
        {
            case Token::Root:             return "Root";

            case Token::Statement:        return "Stmt";
            case Token::StatementList:    return "StmtList";
            case Token::StatementListIf:  return "StmtListIf";
            case Token::StatementListWhile: return "StmtListWhile";
            case Token::Expression:       return "Expr";
            case Token::Expression1:      return "Expr1";
            case Token::Expression2:      return "Expr2";
            case Token::CompareExpression:   return "CompareExpr";
            case Token::VariableDeclaration: return "VarDecl";

            case Token::Id:               return "id";
            case Token::StringLiteral:    return "stringLiteral";
            case Token::NumberLiteral:    return "numberLiteral";

            case Token::String:           return "string";
            case Token::Return:           return "return";
            case Token::While:            return "while";
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