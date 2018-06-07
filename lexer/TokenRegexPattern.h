#pragma once
#include "TokenType.h"

namespace Lexer
{
    struct TokenRegexPattern
    {
        TokenType type;
        std::regex pattern;
    };

    std::string matchRegexPattern(std::regex const& pattern, std::string const& str);
}
