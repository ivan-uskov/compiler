#pragma once
#include "token/Type.h"

namespace Lexer
{
    struct TokenPattern
    {
        Token::Type type;
        std::string pattern;
    };

    bool patternMatches(TokenPattern const& pattern, std::string::iterator const& it, size_t availableSize);
}