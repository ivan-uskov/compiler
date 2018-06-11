#pragma once

#include <vector>
#include <set>
#include <map>
#include <optional>
#include <queue>
#include "Rules.h"

class Generator
{
public:
    enum class CellType
    {
        State, Reduce, None, Ok
    };
    struct TableCell
    {
        CellType type = CellType::None;
        size_t index = 0;

        TableCell() = default;
        explicit TableCell(CellType t, size_t i = 0) : type(t), index(i) {}
    };

    using TableRow = std::map<Token::Type, TableCell>;
    using Table = std::vector<TableRow>;

    explicit Generator(Rules::Table const & t);

    Table getTable() const;
    void printTable(std::ostream & out) const;

private:
    struct StateItem
    {
        Token::Type item;
        size_t row;
        size_t col;

        bool operator < (StateItem const& rhs) const
        {
            return (item < rhs.item) || (row < rhs.row) || (col < rhs.col);
        }

        std::string toString() const
        {
            return Token::tokenTypeToString(item) + std::to_string(row) + std::to_string(col);
        }
    };

    using StateItems = std::set<StateItem>;

    struct State
    {
        StateItems items;
        std::string name;
        size_t row;
    };

    using Lexemes = std::set<Token::Type>;
    using FirstResult = std::map<Token::Type, StateItems>;

    void prepareLexemes();
    void buildTable();

    void processState(std::queue<State> & unprocessed, State const & s);

    std::optional<State> getState(size_t row) const;
    TableRow getStub() const;
    FirstResult first(Token::Type item);
    FirstResult prepareFirstResult() const;
    void fill(std::queue<State> & unprocessed, size_t row, FirstResult const& firstResult);

private:
    std::map<std::string, State> states;
    Rules::Table const& rules;
    Lexemes lexemes;
    Table table;
};