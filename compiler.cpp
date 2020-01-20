#include <iostream>
#include <fstream>

#include "lexer/Tokenizer.h"
#include "slr_parser/Parser.h"
#include "ast_builder/ASTBuilder.h"
#include "ast/View.h"
#include "translation/Interpreter.h"
#include "translation/LLVMCodeGenerator.h"

using namespace std;
using namespace Lexer;

void run(istream & codeIn, istream & programIn, ostream & out)
{
    Tokenizer tokenizer(codeIn, out);
    auto tokens = tokenizer.getTokens();

    ostringstream debug;
    ASTBuilder astBuilder(debug);
    auto ast = astBuilder.build(tokens);

    AST::View av(out);
    ast->accept(av);
    out << endl << endl;

    Translation::Interpreter interpreter(programIn, out);
    ast->accept(interpreter);
    out << endl;

    Translation::LLVMCodeGenerator codeGen;
    codeGen.generate(*ast);
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 2)
        {
            throw logic_error("code input param expected");
        }

        ifstream codeIn(argv[1], std::ifstream::in);
        run(codeIn, cin, cout);
    }
    catch (exception const & e)
    {
        cout << "ERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}