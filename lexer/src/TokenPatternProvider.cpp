#include "lexer/TokenPatternProvider.h"

using namespace std;

namespace Lexer
{
    void TokenPatternProvider::initialize()
    {
        m_patterns = {
                {Token::Type::String,            "string"},
                {Token::Type::Return,            "return"},
                {Token::Type::For,               "for"},
                {Token::Type::Int,               "int"},
                {Token::Type::If,                "if"},

                {Token::Type::And,               "&&"},
                {Token::Type::Or,                "||"},
                {Token::Type::Equals,            "=="},

                {Token::Type::Comma,             ","},
                {Token::Type::Semicolon,         ";"},
                {Token::Type::Assignment,        "="},
                {Token::Type::Less,              "<"},
                {Token::Type::More,              ">"},
                {Token::Type::OpenBrace,        "{"},
                {Token::Type::CloseBrace,       "}"},
                {Token::Type::OpenParenthesis,  "("},
                {Token::Type::CloseParenthesis, ")"},
                {Token::Type::Not,               "!"},
                {Token::Type::Plus,              "+"},
                {Token::Type::Minus,             "-"},
                {Token::Type::Mult,              "*"},
                {Token::Type::Div,               "/"},
                {Token::Type::Mod,               "%"}
        };

        m_regexPatterns = {
                {Token::Type::Identifier,    regex("^([a-zA-Z]+[a-zA-Z0-9]*)(.*)$")},
                {Token::Type::StringLiteral, regex("^(\'[^\']*\')(.*)$")},
                {Token::Type::Number,        regex("^([0-9]+)(.*)$")}
        };
    }

    TokenPatternProvider::TokenPatternVector const & TokenPatternProvider::getPatterns() const
    {
        return m_patterns;
    }

    TokenPatternProvider::TokenRegexPatternVector const & TokenPatternProvider::getRegexPatterns() const
    {
        return m_regexPatterns;
    }
}