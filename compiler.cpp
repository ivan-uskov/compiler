#include <iostream>

#include "lexer/Tokenizer.h"
#include "slr_parser/Parser.h"
#include "ast_builder/ASTBuilder.h"
#include "ast/View.h"
#include "translation/Interpreter.h"
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

            AST::View av(out);
            ast->accept(av);
            out << endl;

            Translation::Interpreter interpreter;
            ast->accept(interpreter);
            out << interpreter.getValue() << endl;
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