#include <iostream>
#include <sstream>

#include "lexer/Tokenizer.h"
#include "parser/Parser.h"

using namespace std;
using namespace Lexer;

int main()
{
    try
    {
        string input = R"(
5 + 6 - 7 * 3
)";

        istringstream in(input);
        Tokenizer tokenizer(in, cerr);
        auto tokens = tokenizer.getTokens();
        for (auto & token : tokens)
        {
            cout << token;
        }

        //Parser p(Rules::get());
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}