#pragma once
#include "TokenType.h"
#include "TokenPattern.h"
#include "TokenRegexPattern.h"

namespace Lexer
{
    class TokenPatternProvider
    {
    public:
        typedef std::vector<TokenPattern> TokenPatternVector;
        typedef std::vector<TokenRegexPattern> TokenRegexPatternVector;

        void initialize();

        TokenPatternVector const& getPatterns() const;
        TokenRegexPatternVector const& getRegexPatterns() const;

    private:
        TokenPatternVector m_patterns;
        TokenRegexPatternVector m_regexPatterns;
        
    };
}