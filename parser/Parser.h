#pragma once

#include "Rules.h"
#include "Generator.h"

class Parser
{
public:
    Parser()
    {
        rulesTable = Rules::get();
        parseTable = Generator(rulesTable).getTable();
    }

private:
    Rules::Table rulesTable;
    Generator::Table parseTable;
};