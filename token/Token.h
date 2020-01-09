#pragma once
#include <string>
#include "Type.h"


namespace Token
{
    struct Token
    {
        Type type;
        std::string value;
        size_t line;
        size_t column;
    };

    std::ostream & operator << (std::ostream & out, Token const& token);
    std::string tokenDescription(Token const& token);
}