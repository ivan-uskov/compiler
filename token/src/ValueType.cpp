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
            case Token::NumberExpression:       return "NumExpr";
            case Token::NumberExpression1:      return "NumExpr1";
            case Token::NumberExpression2:      return "NumExpr2";
            case Token::NumberExpression3:      return "NumExpr3";
            case Token::CompareExpression:   return "CompareExpr";
            case Token::StringExpression:    return "StrExpr";
            case Token::BoolExpression:    return "BoolExpr";
            case Token::VariableDeclaration: return "VarDecl";
            case Token::IdAndOpenSquareBrace:    return "id[";

            case Token::Id:               return "id";
            case Token::StringLiteral:    return "stringLiteral";
            case Token::IntLiteral:       return "intLiteral";
            case Token::DoubleLiteral:    return "doubleLiteral";

            case Token::String:           return "string";
            case Token::Return:           return "return";
            case Token::While:            return "while";
            case Token::Int:              return "int";
            case Token::If:               return "if";
            case Token::Bool:             return "bool";
            case Token::Double:           return "double";

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
            case Token::OpenSquareBrace:  return "[";
            case Token::CloseSquareBrace: return "]";

            case Token::End:              return "end";
            default:                            return "Unexpected token type";
        }
    }
}