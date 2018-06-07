#pragma once
#include "TokenType.h"

namespace Lexer
{
    struct TokenPattern
    {
        TokenType type;
        std::string pattern;
    };

    bool patternMatches(TokenPattern const& pattern, std::string::iterator const& it, size_t availableSize);
}