#include <iostream>

#include "lexer/Tokenizer.h"
#include "slr_parser/Parser.h"

inline std::vector<std::pair<SLR::Rules::Table, std::deque<Token::Token>>> getCases()
{
    return {
            {
                    {
                            {Token::Root, {Token::S}},
                            {Token::S, {Token::A, Token::Else, Token::B, Token::Semicolon}},
                            {Token::A, {Token::i, Token::Semicolon}},
                            {Token::A, {Token::E}},
                            {Token::B, {Token::E}},
                            {Token::E, {Token::E, Token::Plus, Token::i}},
                            {Token::E, {Token::i}}
                    },
                    {{Token::i}, {Token::Plus}, {Token::i}, {Token::Else}, {Token::i}, {Token::Semicolon}, {Token::End}}
            },
            {
                    {
                            {Token::Root, {Token::S}},
                            {Token::S, {Token::A, Token::B,    Token::C}},
                            {Token::A, {Token::A, Token::a}},
                            {Token::A, {}},
                            {Token::B, {Token::B, Token::b}},
                            {Token::B, {}},
                            {Token::C, {Token::c, Token::C}},
                            {Token::C, {}}
                    },
                    {{Token::a}, {Token::b}, {Token::c}, {Token::End}}
            },
            {
                    {
                            {Token::Root, {Token::S}},
                            {Token::S, {Token::a, Token::S,    Token::b}},
                            {Token::S, {Token::A, Token::b, Token::B, Token::c}},
                            {Token::A, {Token::A, Token::c, Token::S}},
                            {Token::A, {}},
                            {Token::B, {}},
                            {Token::B, {Token::B, Token::b}}
                    },
                    {{Token::a}, {Token::b}, {Token::c}, {Token::b}, {Token::End}}
            },
            {
                    {
                            {Token::Root, {Token::S}},
                            {Token::S, {Token::login, Token::d, Token::Semicolon, Token::X, Token::e}},
                            {Token::X, {Token::d, Token::Semicolon, Token::X}},
                            {Token::X, {Token::s, Token::Y}},
                            {Token::Y, {}},
                            {Token::Y, {Token::Semicolon, Token::s, Token::Y}}
                    },
                    {{Token::login}, {Token::d}, {Token::Semicolon}, {Token::d}, {Token::Semicolon}, {Token::s}, {Token::Semicolon}, {Token::s}, {Token::e}, {Token::End}}
            },
            {
                    {
                            {Token::Root, {Token::S}},
                            {Token::S, {Token::A}},
                            {Token::S, {Token::While, Token::E, Token::Do, Token::S}},
                            {Token::E, {Token::E, Token::Plus, Token::E}},
                            {Token::E, {Token::Id}},
                            {Token::A, {}},
                            {Token::A, {Token::Id, Token::Assignment, Token::E}}
                    },
                    {{Token::While}, {Token::Id}, {Token::Plus}, {Token::Id}, {Token::Do}, {Token::Id}, {Token::Assignment}, {Token::Id}, {Token::End}}
            },
            {
                    {
                            {Token::Root, {Token::S}},
                            {Token::S, {Token::a, Token::A, Token::a}},
                            {Token::S, {Token::a, Token::B, Token::b}},
                            {Token::S, {Token::b, Token::B, Token::a}},
                            {Token::S, {Token::b, Token::A, Token::b}},
                            {Token::A, {Token::c}},
                            {Token::B, {Token::c}}
                    },
                    {{Token::a}, {Token::c}, {Token::b}, {Token::End}}
            },
            {
                    {
                            {Token::Root, {Token::S}},
                            {Token::S, {Token::A, Token::A}},
                            {Token::A, {Token::a, Token::A}},
                            {Token::A, {Token::b}}
                    },
                    {{Token::a}, {Token::b}, {Token::b}, {Token::End}}
            },
            {
                    {
                            {Token::Root, {Token::S}},
                            {Token::S, {Token::S, Token::S}},
                            {Token::S, {Token::A}},
                            {Token::A, {Token::i, Token::A}},
                            {Token::A, {Token::i}}
                    },
                    {{Token::i}, {Token::i}, {Token::i}, {Token::End}}
            },
            {
                    {
                            {Token::Root, {Token::S}},
                            {Token::S, {Token::A, Token::B}},
                            {Token::A, {Token::a, Token::A}},
                            {Token::A, {Token::a, Token::C}},
                            {Token::A, {}},
                            {Token::B, {Token::B, Token::b}},
                            {Token::B, {Token::C, Token::b}},
                            {Token::B, {}},
                            {Token::C, {Token::c}},
                            {Token::C, {}}
                    },
                    {{Token::a}, {Token::a}, {Token::c}, {Token::b}, {Token::b}, {Token::End}}
            }
    };
}

using namespace std;

void test(SLR::Rules::Table const & r, deque<Token::Token> & tt)
{
    SLR::Parser(r, cout).parse(tt);
    cout << endl;
}

void testParser()
{
    auto cases = getCases();
    for (auto & c : cases)
    {
        test(c.first, c.second);
    }
}