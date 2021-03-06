#include "lexer/TokenPatternProvider.h"

using namespace std;

namespace Lexer
{
    void TokenPatternProvider::initialize()
    {
        m_patterns = {
                {Token::String,       "string"},
                {Token::Return,       "return"},
                {Token::While,        "while"},
                {Token::Int,          "int"},
                {Token::If,           "if"},
                {Token::Bool,         "bool"},
                {Token::Double,        "double"},

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
                {Token::Div,              "/"},
                {Token::Mod,              "%"},
                {Token::OpenSquareBrace,  "["},
                {Token::CloseSquareBrace, "]"}
        };

        m_regexPatterns = {
                {Token::BoolLiteral,   regex("^(true|false)(.*)$")},
                {Token::Id,            regex("^([a-zA-Z]+[a-zA-Z0-9]*)(.*)$")},
                {Token::StringLiteral, regex(R"(^('[^']*')(.*)$)")},
                {Token::DoubleLiteral, regex("^([0-9]+\.[0-9]+)(.*)$")},
                {Token::IntLiteral,    regex("^([0-9]+)(.*)$")}
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