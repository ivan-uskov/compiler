#pragma once

#include <map>
#include <set>
#include "Rules.h"

class Generator
{
public:
    using TableItem = size_t;
    using TableRow = std::map<Rules::Item, TableItem>;
    using Table = std::map<TableItem, TableRow>;

    static const size_t RowWidth;
    static const size_t Ok;
    static const size_t None;
    static const size_t FirstRowIndex;

    static Table buildTable(Rules::Table const & rules);

private:
    using Lexemes = std::set<Rules::Item>;

    static TableRow getStub(Lexemes const & lexemes);
    static std::set<Rules::Item> getLexemes(Rules::Table const & table);
    static size_t absoluteItemIndex(size_t row, size_t col);
    static Rules::Item toRuleItem(Rules::Table const & table, TableItem index);
    static std::pair<TableItem, TableItem> posFromTableItem(TableItem index);
    static std::set<TableItem> first(Rules::Table const & table, Rules::Item item);
    static std::set<TableItem> first2(Rules::Table const & table, TableItem item);
    static void fillTableRow(Rules::Table const& rules, TableRow & row, std::set<TableItem> const& items);
};