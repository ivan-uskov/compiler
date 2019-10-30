#pragma once

#include <vector>
#include <set>
#include <map>
#include <experimental/optional>
#include <queue>
#include "Rules.h"

namespace SLR
{
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

        explicit Generator(Rules::Table const & t, std::ostream & debug);

        Table getTable() const;

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

        template<typename T>
        using Optional = std::experimental::optional<T>;
        using NullableState = Optional<Generator::State>;

        using Lexemes = std::set<Token::Type>;
        using FirstResult = std::map<Token::Type, StateItems>;
        struct FirstPlusResult
        {
            FirstResult fr;
            std::map<size_t, std::set<Token::Type>> reduces;
        };

    private:
        void prepareLexemes();
        void prepareEmpties();
        void prepareFollowings();
        std::set<Token::Type> prepareFollowing(Token::Type, std::set<Token::Type> & processed) const;
        void buildTable();
        void printTable(std::ostream & out) const;

        void processState(std::queue<State> & unprocessed, State const & s);

        NullableState getState(size_t row) const;
        TableRow getStub() const;
        FirstResult first(Token::Type item) const;
        FirstPlusResult firstPlus(Token::Type const& item) const;
        FirstResult prepareFirstResult() const;
        void fill(std::queue<State> & unprocessed, size_t row, FirstPlusResult const& frp);

    private:
        std::map<Token::Type, std::set<Token::Type>> followings;
        std::map<std::string, State> states;
        Rules::Table const& rules;
        Lexemes lexemes;
        Table table;
        std::ostream & debug;
        std::map<Token::Type, Optional<size_t>> empties;
    };
}