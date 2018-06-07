#include "lexer/TokenPattern.h"

using namespace std;

namespace Lexer
{
    bool patternMatches(TokenPattern const& pattern, std::string::iterator const& it, size_t availableSize)
    {
        if (pattern.pattern.size() > availableSize)
        {
            return false;
        }

        for (size_t i = 0; i < pattern.pattern.size(); ++i)
        {
            if (pattern.pattern[i] != it[i])
            {
                return false;
            }
        }

        return true;
    }
}