#pragma once
#include <map>
#include <set>
#include <limits>
#include <queue>
#include <algorithm>
#include "Rules.h"

class Generator
{
public:
    using TableItem = size_t;
    using TableRow = std::map<Rules::Item, TableItem>;
    using Table = std::map<TableItem, TableRow>;

    using Lexemes = std::set<Rules::Item>;

    static const size_t RowWidth = 10;
    static const size_t Ok = std::numeric_limits<size_t>::max();
    static const size_t None = std::numeric_limits<size_t>::max() - 1;
    static const size_t FirstRowIndex = std::numeric_limits<size_t>::max() - 2;

    static Table buildTable(Rules::Table const& t)
    {
        auto lexemes = getLexemes(t);
        std::queue<TableItem> unprocessed;
        Table table;
        auto initialItems = first(t, Rules::Root);
        std::copy(initialItems.begin(), initialItems.end(), std::back_inserter(unprocessed));
        table[FirstRowIndex] = getStub(lexemes);
        table[FirstRowIndex][Rules::Root] = Ok;

        for (auto & tableItem : initialItems)
        {
            table[FirstRowIndex][toRuleItem(t, tableItem)] = tableItem;
        }

        while (!unprocessed.empty())
        {

        }
    }

    static TableRow getStub(Lexemes const& lexemes)
    {
        TableRow row;
        for (auto & l : lexemes)
        {
            row[l] = None;
        }

        return row;
    }

    static std::set<Rules::Item> getLexemes(Rules::Table const &table)
    {
        std::set<Rules::Item> lexems;
        for (auto & rule : table)
        {
            lexems.insert(rule.first);
            for (auto & rightItem : rule.second)
            {
                lexems.insert(rightItem);
            }
        }

        lexems.insert(Rules::Eof);

        return lexems;
    }

    static size_t absoluteItemIndex(size_t row, size_t col)
    {
        return row * RowWidth + col;
    }

    static Rules::Item toRuleItem(Rules::Table const &table, TableItem index)
    {
        auto offset = itemFromAbsoluteIndex(index);
        return table[offset.first].second[offset.second];
    }

    static std::pair<TableItem, TableItem> itemFromAbsoluteIndex(TableItem index)
    {
        TableItem rows = index / RowWidth;
        return {rows, index - rows * RowWidth};
    };

    static std::set<TableItem> first(Rules::Table const& table, Rules::Item item)
    {
        std::set<TableItem> items;

        for (size_t row = 0; row < table.size(); ++row)
        {
            auto const& rule = table[row];
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

    static std::set<TableItem> first2(Rules::Table const& table, Rules::Item item)
    {
        std::set<TableItem> items;

        for (size_t row = 0; row < table.size(); ++row)
        {
            auto const& rule = table[row];
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
};