#include <regex>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <iostream>

#include "lexer/Tokenizer.h"
#include "parser/Generator.h"

using namespace std;
using namespace Lexer;

int main()
{
    try
    {
        Tokenizer tokenizer(cin, cerr);
        tokenizer.scan();
        auto tokens = tokenizer.getTokens();
        for (auto & token : tokens)
        {
            cout << token;
        }
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}