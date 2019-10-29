#include <iostream>

#include "lexer/Tokenizer.h"
#include "parser/Parser.h"
#include "ast_builder/ASTBuilder.h"
#include "ast/ASTView.h"
#include "parser_tests.h"

using namespace std;
using namespace Lexer;

void testWithLexer()
{
    string input = "5 + 7 * 4 * 4";
    cout << input << endl;
    istringstream in(input);

    Tokenizer tokenizer(in, cerr);
    auto tokens = tokenizer.getTokens();

    ASTBuilder pc;
    Parser(pc.getRules(), cout).parse(tokens);
    cout << endl;
    auto ast = pc.getAST();
    ASTView av(std::cout);
    ast->accept(av);
}

int main()
{
    try
    {
        testParser();
        testWithLexer();
    }
    catch (exception const & e)
    {
        cout << "ERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}