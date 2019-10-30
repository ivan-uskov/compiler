#include <iostream>

#include "lexer/Tokenizer.h"
#include "parser/Parser.h"
#include "ast_builder/ASTBuilder.h"
#include "ast/ASTView.h"
#include "parser_tests.h"

using namespace std;
using namespace Lexer;

void run(istream & in, ostream & out)
{
    while (!in.eof())
    {
        string line;
        getline(in, line);
        if (!line.empty())
        {
            istringstream lineStrm(line);
            Tokenizer tokenizer(lineStrm, out);
            auto tokens = tokenizer.getTokens();

            ASTBuilder astBuilder(cerr);
            auto ast = astBuilder.build(tokens);

            ASTView av(out);
            ast->accept(av);
            out << endl;
        }
    }
}

int main()
{
    try
    {
        //testParser();
        run(cin, cout);
    }
    catch (exception const & e)
    {
        cout << "ERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}