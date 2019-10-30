#pragma once

#include <vector>
#include <functional>
#include "token/Type.h"
#include "token/Token.h"

namespace SLR
{
    class Rules
    {
    public:
        using Action = std::function<void(std::vector<Token::Token>)>;
        struct Rule
        {
            Token::Type first;
            std::vector<Token::Type> second;
            Action action;
        };
        using Item = Token::Type;
        using Table = std::vector<Rule>;
    };
}