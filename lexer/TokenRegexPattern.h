#pragma once
#include "token/Type.h"

namespace Lexer
{
    struct TokenRegexPattern
    {
        Token::Type type;
        std::regex pattern;
    };

    std::string matchRegexPattern(std::regex const& pattern, std::string const& str);
}
