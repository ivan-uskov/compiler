#include "lexer/TokenPatternProvider.h"

using namespace std;

namespace Lexer
{
    void TokenPatternProvider::initialize()
    {
        m_patterns = {
                {Token::String,       "string"},
                {Token::Return,       "return"},
                {Token::While,        "While"},
                {Token::Number,       "number"},
                {Token::If,           "if"},

                {Token::And,          "&&"},
                {Token::Or,           "||"},
                {Token::DoubleEquals, "=="},

                {Token::Comma,        ","},
                {Token::Semicolon,    ";"},
                {Token::Equals,       "="},
                {Token::Less,             "<"},
                {Token::More,             ">"},
                {Token::OpenBrace,        "{"},
                {Token::CloseBrace,       "}"},
                {Token::OpenParenthesis,  "("},
                {Token::CloseParenthesis, ")"},
                {Token::Not,              "!"},
                {Token::Plus,             "+"},
                {Token::Minus,            "-"},
                {Token::Mult,             "*"},
                {Token::Div,               "/"},
                {Token::Mod,               "%"}
        };

        m_regexPatterns = {
                {Token::Id,    regex("^([a-zA-Z]+[a-zA-Z0-9]*)(.*)$")},
                {Token::StringLiteral, regex("^(\'[^\']*\')(.*)$")},
                {Token::NumberLiteral, regex("^([0-9]+)(.*)$")}
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