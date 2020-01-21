#include <iostream>
#include <fstream>

#include "lexer/Tokenizer.h"
#include "slr_parser/Parser.h"
#include "ast_builder/ASTBuilder.h"
#include "ast/View.h"
#include "translation/Interpreter.h"
#include "translation/FakeLLVMCodeGenerator.h"

using namespace std;
using namespace Lexer;

void run(istream & codeIn, istream & programIn, ostream & out)
{
    Tokenizer tokenizer(codeIn, out);
    auto tokens = tokenizer.getTokens();

    ostringstream debug;
    ASTBuilder astBuilder(debug);
    auto ast = astBuilder.build(tokens);

    Translation::Interpreter interpreter(programIn, out);
    ast->accept(interpreter);
    out << endl;
}

void view(istream & in, ostream & out)
{
    Tokenizer tokenizer(in, out);
    auto tokens = tokenizer.getTokens();

    ostringstream debug;
    ASTBuilder astBuilder(debug);
    auto ast = astBuilder.build(tokens);

    AST::View av(out);
    ast->accept(av);
    out << endl << endl;
}

void compile(istream & in, ostream & out)
{
    Tokenizer tokenizer(in, out);
    auto tokens = tokenizer.getTokens();

    ostringstream debug;
    ASTBuilder astBuilder(debug);
    auto ast = astBuilder.build(tokens);

    Translation::FakeLLVMCodeGenerator codeGen;
    out << codeGen.generate(*ast) << endl;
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc == 3)
        {
            ifstream codeIn(argv[2], std::ifstream::in);
            if (argv[1] == string("view"))
            {
                view(codeIn, cout);
            }
            else if (argv[1] == string("run"))
            {
                run(codeIn, cin, cout);
            }
        }
        else if (argc == 2)
        {
            if (argv[1] == string("view"))
            {
                view(cin, cout);
            }
            else
            {
                ifstream codeIn(argv[1], std::ifstream::in);
                run(codeIn, cin, cout);
            }
        }
        else
        {
            compile(cin, cout);
        }
    }
    catch (exception const & e)
    {
        cout << "[ERROR]: " << e.what() << endl;
        return 1;
    }

    return 0;
}