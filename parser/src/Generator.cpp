#include <limits>
#include <algorithm>
#include <functional>

#include "parser/Generator.h"

using namespace std;

Generator::Generator(Rules::Table const & t, std::ostream & d)
    : rules(t)
    , debug(d)
{
    prepareLexemes();
    prepareFollowings();
    buildTable();
    printTable(debug);
}

Generator::Table Generator::getTable() const
{
    return table;
}

void Generator::printTable(std::ostream & out) const
{
    for (auto & rule : rules)
    {
        out << Token::tokenTypeToString(rule.first) << " -> ";
        for (auto & item : rule.second)
        {
            out << Token::tokenTypeToString(item) << " ";
        }
        out << endl;
    }

    auto print = [&out](string const& str) {
        out << str << (str.size() <= 7 ? "\t\t" : "\t");
    };

    print("State");
    for (auto & l : lexemes)
    {
        print(Token::tokenTypeToString(l));
    }
    out << endl;
    for (size_t row = 0; row < table.size() ; ++row)
    {
        auto it = find_if(states.begin(), states.end(), [&](auto & item){ return item.second.row == row; });
        if (it != states.end())
        {
            print(it->second.name);
        }
        else
        {
            print("S" + to_string(row));
        }

        for (auto & cell : table[row])
        {
            if (cell.second.type == CellType::Ok)
            {
                print("Ok");
            }
            else if (cell.second.type == CellType::None)
            {
                print("--");
            }
            else if (cell.second.type == CellType::Reduce)
            {
                print("R" + to_string(cell.second.index));
            }
            else if (cell.second.type == CellType::State)
            {
                auto state = getState(cell.second.index);
                if (state)
                {
                    print(state->name);
                }
            }
        }
        out << endl;
    }
}

Generator::NullableState Generator::getState(size_t row) const
{
    auto stateIt = find_if(states.begin(), states.end(), [row](auto & item) {
        return item.second.row == row;
    });
    if (stateIt == states.end())
    {
        return {};
    }

    return stateIt->second;
}

void Generator::buildTable()
{
    auto initialItems = first(Token::Root);

    table.emplace_back(getStub());
    table[0][Token::Root] = TableCell(CellType::Ok);

    std::queue<State> unprocessed;
    fill(unprocessed, 0, initialItems);

    while (!unprocessed.empty())
    {
        auto state = unprocessed.front();
        unprocessed.pop();

        processState(unprocessed, state);
    }
}

void Generator::processState(std::queue<State> & unprocessed, State const & s)
{
    for (auto & item : s.items)
    {
        auto rule = rules[item.row];
        auto nextIndex = item.col + 1;
        if (rule.second.size() == nextIndex)
        {
            if (!followings[rule.first].empty())
            {
                for (auto & fw : followings[rule.first])
                {
                    table[s.row][fw] = TableCell(CellType::Reduce, item.row);
                }
            }
            else
            {
                table[s.row][Token::End] = TableCell(CellType::Reduce, item.row);
            }
        }
        else
        {
            auto next = rule.second[nextIndex];
            auto stateItem = StateItem{next, item.row, nextIndex};
            FirstResult fr;
            if (Token::isLiteral(next))
            {
                fr = first(next);
                fr[next].insert(stateItem);

            }
            else
            {
                fr = prepareFirstResult();
                fr[next].insert(stateItem);
            }

            fill(unprocessed, s.row, fr);
        }
    }
}

void Generator::fill(std::queue<State> & unprocessed, size_t row, FirstResult const& firstResult)
{
    auto buildState = [](StateItems const& stateItems) -> State {
        string name;
        for (auto & stateItem : stateItems)
        {
            name += stateItem.toString();
        }
        return {stateItems, name, 0};
    };

    for (auto & item : firstResult)
    {
        auto state = buildState(item.second);
        if (state.name.empty())
        {
            continue;
        }

        auto existingStateIt = states.find(state.name);
        if (existingStateIt == states.end())
        {
            state.row = table.size();
            table.emplace_back(getStub());
            states[state.name] = state;
            unprocessed.push(state);
        }
        else
        {
            state.row = existingStateIt->second.row;
        }
        table[row][item.first] = TableCell(CellType::State, state.row);
    }
}

Generator::TableRow Generator::getStub() const
{
    TableRow row;
    for (auto & l : lexemes)
    {
        row[l] = {};
    }

    return row;
}

Generator::FirstResult Generator::prepareFirstResult() const
{
    FirstResult items;
    for (auto & stubItem : getStub())
    {
        items[stubItem.first] = {};
    }
    return items;
}

void Generator::prepareLexemes()
{
    for (auto & rule : rules)
    {
        lexemes.insert(rule.first);
        for (auto & rightItem : rule.second)
        {
            lexemes.insert(rightItem);
        }
    }

    lexemes.insert(Token::End);
}

Generator::FirstResult Generator::first(Token::Type item) const
{
    auto items = prepareFirstResult();

    for (size_t row = 0; row < rules.size(); ++row)
    {
        auto const & rule = rules[row];
        if (rule.first == item && !rule.second.empty())
        {
            auto val = rule.second.front();
            auto stateItem = StateItem{val, row, 0};
            items[val].insert(stateItem);
            if (Token::isLiteral(val) && (val != item))
            {
                auto subValues = first(val);
                for (auto & subItem : subValues)
                {
                    for (auto & i : subItem.second)
                    {
                        items[subItem.first].insert(i);
                    }
                }
            }
        }
    }

    return items;
}

void Generator::prepareFollowings()
{
    for (auto & r1 : rules)
    {
        std::set<Token::Type> processed;
        followings[r1.first] = prepareFollowing(r1.first, processed);
    }
}

std::set<Token::Type> Generator::prepareFollowing(Token::Type t, std::set<Token::Type> & processed) const
{
    std::set<Token::Type> items;
    processed.insert(t);

    for (auto & rule : rules)
    {
        auto ruleSize = rule.second.size();
        for (size_t i = 0; i < ruleSize; ++i)
        {
            auto nextI = i + 1;
            if ((rule.second[i] == t))
            {
                if (nextI < ruleSize)
                {
                    items.insert(rule.second[nextI]);
                }
                else if (rule.first ==  Token::Root)
                {
                    items.insert(Token::End);
                }

                if (processed.find(rule.first) == processed.end())
                {
                    auto newItems = prepareFollowing(rule.first, processed);
                    items.insert(newItems.begin(), newItems.end());
                }
            }
        }
    }

    return items;
}