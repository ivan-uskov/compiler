#include <iostream>
#include "token/Token.h"

using namespace std;

namespace Token
{
    ostream & operator << (ostream & out, Token const& token)
    {
        return out << tokenTypeToString(token.type) << "\t\t[" << token.value << "]\t\tat\t" << token.line << ":" << token.column;
    }
}

