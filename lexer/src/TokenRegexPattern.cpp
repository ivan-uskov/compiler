#include <string>
#include <regex>
#include "lexer/TokenRegexPattern.h"

using namespace std;

namespace Lexer
{
    string matchRegexPattern(regex const& pattern, std::string const& str)
    {
        string result;
        smatch matches;

        if (regex_match(str, matches, pattern))
        {
            result = matches[1].str();
        }

        return result;
    }
}