#include <iostream>
#include <sstream>
#include <optional>

#include "lexer/Tokenizer.h"
#include "parser/Parser.h"
#include "NullStream.h"

using namespace std;
using namespace Lexer;

int main()
{
    try
    {
        string input = "5 + 7 * 4";
        cout << input << endl;
        istringstream in(input);
        Tokenizer tokenizer(in, cerr);
        auto tokens = tokenizer.getTokens();

        auto null = NullStream::provider();
        Parser(null()).parse(tokens);
    }
    catch (exception const& e)
    {
        cout << "ERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}