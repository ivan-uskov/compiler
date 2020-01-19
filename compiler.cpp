#include <iostream>

#include "lexer/Tokenizer.h"
#include "slr_parser/Parser.h"
#include "ast_builder/ASTBuilder.h"
#include "ast/View.h"
#include "translation/Interpreter.h"
#include "translation/LLVMCodeGenerator.h"

using namespace std;
using namespace Lexer;

void run(istream & in, ostream & out)
{
    Tokenizer tokenizer(in, out);
    auto tokens = tokenizer.getTokens();

    ostringstream debug;
    ASTBuilder astBuilder(debug);
    auto ast = astBuilder.build(tokens);

    AST::View av(out);
    ast->accept(av);
    out << endl;

    Translation::Interpreter interpreter(in, out);
    ast->accept(interpreter);
    out << endl;

    Translation::LLVMCodeGenerator codeGen;
    out << codeGen.generate(*ast) << endl;
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