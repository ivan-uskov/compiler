#pragma once
#include "Token.h"
#include "TokenPatternProvider.h"
#include "TokenType.h"
#include <string>
#include <iostream>
#include <functional>

namespace Lexer
{
    class Tokenizer
    {
    public:
        Tokenizer(std::istream & input, std::ostream & errors);

        bool scan();
        std::vector<Token> getTokens() const;

    private:
        bool parseLine();
        bool parseAtom(std::string::iterator const& it);
        bool parseToken(std::string::iterator const& it, size_t availableSize);
        bool parseRegexTokens(std::string const& line);

        void trimLine();
        void addToken(TokenType const& type, std::string const& value = "");
        void addEndToken();
        void addTokenByPattern(Lexer::TokenPattern const& pattern);
        void addInvalidToken();

        std::string m_currentLine;
        size_t m_lineCount = 0;
        size_t m_charCount = 0;

        std::istream & m_input;
        std::ostream & m_errors;
        std::vector<Token> m_tokens;
        TokenPatternProvider m_tokenPatternProvider;
    };
}
