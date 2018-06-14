#include <iostream>
#include <sstream>

#include "lexer/Tokenizer.h"
#include "parser/Parser.h"
#include "NullStream.h"

using namespace std;
using namespace Lexer;

int main()
{
    try
    {
/*
        string input = "5 + 7 * 4 * 4";
        cout << input << endl;
        istringstream in(input);

        Tokenizer tokenizer(in, cerr);
        auto tokens = tokenizer.getTokens();
        Parser(Rules::get(), cout).parse(tokens);
        cout << endl;

        auto r1 = Rules::Table{
            {Token::Root, {Token::S}},
            {Token::S, {Token::A, Token::Else, Token::B, Token::Semicolon}},
            {Token::A, {Token::i, Token::Semicolon}},
            {Token::A, {Token::E}},
            {Token::B, {Token::E}},
            {Token::E, {Token::E, Token::Plus, Token::i}},
            {Token::E, {Token::i}}
        };
        auto t1 = std::deque<Token::Token>{
            {Token::i}, {Token::Plus}, {Token::i}, {Token::Else}, {Token::i}, {Token::Semicolon}, {Token::End}
        };
        Parser(r1, cout).parse(t1);
        cout << endl;
*/
        auto r2 = Rules::Table{
                {Token::Root, {Token::S}},
                {Token::S, {Token::A, Token::B, Token::C}},
                {Token::A, {Token::A, Token::a}},
                {Token::A, {}},
                {Token::B, {Token::B, Token::b}},
                {Token::B, {}},
                {Token::C, {Token::c, Token::C}},
                {Token::C, {}}
        };
        auto t2 = std::deque<Token::Token>{
                {Token::a}, {Token::b}, {Token::c}, {Token::End}
        };
        Parser(r2, cout).parse(t2);
    }
    catch (exception const& e)
    {
        cout << "ERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}