#include <iostream>
#include <sstream>
#include "token/Token.h"

using namespace std;

namespace Token
{
    ostream & operator << (ostream & out, Token const& token)
    {
        return out << tokenDescription(token);
    }

    std::string tokenDescription(Token const& token)
    {
        ostringstream ostr;
        ostr << tokenTypeToString(token.type) << "\t\t[" << token.value << "]\t\tat\t" << token.line << ":" << token.column;
        return ostr.str();
    }
}