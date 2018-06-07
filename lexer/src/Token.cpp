#include <iostream>
#include "lexer/Token.h"

using namespace std;
using namespace Lexer;

ostream & operator << (ostream & out, Token const& token)
{
    return out << tokenTypeToString(token.type) << "\t\t[" << token.value << "]\t\tat\t" << token.line << ":" << token.column << endl;
}