#pragma once
#include <string>
#include "TokenType.h"

namespace Lexer
{
    struct Token
    {
        TokenType type;
        std::string value;
        size_t line;
        size_t column;
    };
}

std::ostream & operator << (std::ostream & out, Lexer::Token const& token);