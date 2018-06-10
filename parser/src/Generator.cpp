#include <limits>
#include <queue>
#include <algorithm>
#include <functional>

#include "parser/Generator.h"

const size_t Generator::RowWidth = 10;
const size_t Generator::Ok = std::numeric_limits<size_t>::max();
const size_t Generator::None = std::numeric_limits<size_t>::max() - 1;
const size_t Generator::FirstRowIndex = std::numeric_limits<size_t>::max() - 2;

Generator::Table Generator::buildTable(Rules::Table const & rules)
{
    auto lexemes = getLexemes(rules);
    auto initialItems = first(rules, Rules::Item::Root);

    Table table;
    table[FirstRowIndex] = getStub(lexemes);
    table[FirstRowIndex][Rules::Item::Root] = Ok;
    fillTableRow(rules, table[FirstRowIndex], initialItems);

    std::queue<TableItem> unprocessed;
    std::for_each(initialItems.begin(), initialItems.end(), [&](auto & item) { unprocessed.push(item); });

    while (!unprocessed.empty())
    {
        auto item = unprocessed.front();
        unprocessed.pop();

        auto items = first2(rules, item);
        if (items.empty())
        {
            table[item] = {};
        }
        else
        {
            table[item] = getStub(lexemes);
            fillTableRow(rules, table[item], items);
        }

        std::for_each(items.begin(), items.end(), [&](auto & item) {
            if (table.find(item) == table.end())
            {
                unprocessed.push(item);
            }
        });
    }

    return table;
}

void Generator::fillTableRow(Rules::Table const& rules, TableRow & row, std::set<TableItem> const& items)
{
    for (auto & tableItem : items)
    {
        row[toRuleItem(rules, tableItem)] = tableItem;
    }
}

Generator::TableRow Generator::getStub(Lexemes const & lexemes)
{
    TableRow row;
    for (auto & l : lexemes)
    {
        row[l] = None;
    }

    return row;
}

std::set<Rules::Item> Generator::getLexemes(Rules::Table const & table)
{
    std::set<Rules::Item> lexemes;
    for (auto & rule : table)
    {
        lexemes.insert(rule.first);
        for (auto & rightItem : rule.second)
        {
            lexemes.insert(rightItem);
        }
    }

    lexemes.insert(Rules::Item::End);

    return lexemes;
}

size_t Generator::absoluteItemIndex(size_t row, size_t col)
{
    return row * RowWidth + col;
}

Rules::Item Generator::toRuleItem(Rules::Table const & table, TableItem index)
{
    auto offset = posFromTableItem(index);
    return table[offset.first].second[offset.second];
}

std::pair<Generator::TableItem, Generator::TableItem> Generator::posFromTableItem(TableItem index)
{
    TableItem rows = index / RowWidth;
    return {rows, index - rows * RowWidth};
};

std::set<Generator::TableItem> Generator::first(Rules::Table const & table, Rules::Item item)
{
    std::set<TableItem> items;

    for (size_t row = 0; row < table.size(); ++row)
    {
        auto const & rule = table[row];
        if (rule.first == item)
        {
            items.insert(absoluteItemIndex(row, 0));
            auto val = rule.second.front();
            if (Rules::isLiteral(val))
            {
                auto subValues = first(table, val);
                items.insert(subValues.begin(), subValues.end());
            }
        }
    }

    return items;
}

std::set<Generator::TableItem> Generator::first2(Rules::Table const & table, TableItem item)
{
    auto pos = posFromTableItem(item);
    auto rule = table.at(pos.first);
    auto nextIndex = pos.second + 1;

    if (nextIndex == rule.second.size())
    {
        return {};
    }

    std::set<TableItem> items;
    items.insert(absoluteItemIndex(pos.first, nextIndex));
    auto val = rule.second.at(nextIndex);
    if (Rules::isLiteral(val))
    {
        auto subValues = first(table, val);
        items.insert(subValues.begin(), subValues.end());
    }

    return items;
}