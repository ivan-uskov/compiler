#pragma once

#include "Rules.h"
#include "Generator.h"

class Parser
{
public:
    explicit Parser(Rules::Table const& rulesTable)
        : rulesTable(rulesTable)
    {
        parseTable = Generator::buildTable(rulesTable);
    }

private:
    Rules::Table const& rulesTable;
    Generator::Table parseTable;
};