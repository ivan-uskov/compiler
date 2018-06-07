#include "lexer/TokenPatternProvider.h"

using namespace std;

namespace Lexer
{
    void TokenPatternProvider::initialize()
    {
        m_patterns = {
            { TokenType::STRING, "string" },
            { TokenType::RETURN, "return" },
            { TokenType::FOR,    "for"    },
            { TokenType::INT,    "int"    },
            { TokenType::IF,     "if"     },

            { TokenType::AND,    "&&" },
            { TokenType::OR,     "||" },
            { TokenType::EQUALS, "==" },

            { TokenType::COMMA,             "," },
            { TokenType::SEMICOLON,         ";" },
            { TokenType::ASSIGNMENT,        "=" },
            { TokenType::LESS,              "<" },
            { TokenType::MORE,              ">" },
            { TokenType::OPEN_BRACE,        "{" },
            { TokenType::CLOSE_BRACE,       "}" },
            { TokenType::OPEN_PARENTHESIS,  "(" },
            { TokenType::CLOSE_PARENTHESIS, ")" },
            { TokenType::NOT,               "!" },
            { TokenType::PLUS,              "+" },
            { TokenType::MINUS,             "-" },
            { TokenType::MULT,              "*" },
            { TokenType::DIV,               "/" },
            { TokenType::MOD,               "%" }
        };

        m_regexPatterns = {
            { TokenType::IDENTIFICATOR,  regex("^([a-zA-Z]+[a-zA-Z0-9]*)(.*)$") },
            { TokenType::STRING_LITERAL, regex("^(\'[^\']*\')(.*)$")            },
            { TokenType::NUMBER,         regex("^([0-9]+)(.*)$")                }
        };
    }

    TokenPatternProvider::TokenPatternVector const& TokenPatternProvider::getPatterns() const
    {
        return m_patterns;
    }

    TokenPatternProvider::TokenRegexPatternVector const& TokenPatternProvider::getRegexPatterns() const
    {
        return m_regexPatterns;
    }
}