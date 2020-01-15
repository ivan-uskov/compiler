#pragma once

#include <functional>
#include <stdexcept>

#include "Rules.h"
#include "Generator.h"
#include "token/Token.h"

namespace SLR
{
    class Parser
    {
    public:
        explicit Parser(Rules::Table const& t, std::ostream & d)
                : debug(d)
        {
            rulesTable = t;
            parseTable = Generator(rulesTable, debug).getTable();
        }

        void parse(std::deque<Token::Token> & input)
        {
            states.push(0);

            while (true)
            {
                auto next = parseTable[states.top()][input.front().type];
                if (next.type == Generator::CellType::State)
                {
                    shift(next.index, input);
                }
                else if (next.type == Generator::CellType::Reduce)
                {
                    reduce(next.index, input);
                }
                else if (next.type == Generator::CellType::Ok)
                {
                    finish(input);
                    break;
                }
                else if (next.type == Generator::CellType::None)
                {
                    throw std::logic_error(getNoneErrorMessage(input.front().type));
                }
            }
        }

    private:

        std::string getNoneErrorMessage(Token::Type next) const
        {
            std::ostringstream out;
            out << "after " << (tokens.empty() ? "--" : Token::tokenDescription(tokens.top())) << " expected ";

            for (auto & i : parseTable[states.top()])
            {
                if (i.second.type != Generator::CellType::None)
                {
                    out << Token::tokenTypeToString(i.first).c_str() << " ";
                }
            }
            out << "got " << Token::tokenTypeToString(next);

            return out.str();
        }

        void finish(std::deque<Token::Token> & input)
        {
            using namespace std;
            if (!tokens.empty())
            {
                ostringstream out;
                while (!tokens.empty())
                {
                    auto t = tokens.top();
                    tokens.pop();
                    out << Token::tokenTypeToString(t.type) << " ";
                }
                throw std::logic_error("tokens not empty");
            }
            if (states.size() != 1 && (states.top() != 0))
            {
                ostringstream out;
                while (!states.empty())
                {
                    auto s = states.top();
                    states.pop();
                    out << "S" << s << " ";
                }
                throw std::logic_error("invalid states: " + out.str());
            }

            if ((input.size() != 2) || (input.front().type != Token::Root))
            {
                ostringstream out;
                while (!input.empty())
                {
                    auto s = input.front();
                    input.pop_front();
                    out << Token::tokenTypeToString(s.type) << " ";
                }
                throw std::logic_error("Invalid input chars: " + out.str());
            }

            debug << "Ok" << std::endl;
        }

        void shift(size_t stateIndex, std::deque<Token::Token> & input)
        {
            states.push(stateIndex);
            tokens.push(input.front());
            input.pop_front();

            debug << "shift to S" << states.top() << std::endl;
            printInfo(input);
        }

        void reduce(size_t ruleIndex, std::deque<Token::Token> & input)
        {
            auto rule = rulesTable[ruleIndex];

            std::vector<Token::Token> ruleTokens;
            ruleTokens.reserve(rule.second.size());
            auto popSize = rule.second.size();
            while (popSize-- > 0)
            {
                ruleTokens.push_back(tokens.top());
                tokens.pop();
                states.pop();
            }

            input.push_front(Token::Token{rule.first, "" , 0, 0});
            if (rule.action)
            {
                rule.action(ruleTokens);
            }

            debug << "reduce" << ruleIndex << std::endl;

            printInfo(input);
        }

        void printInfo(std::deque<Token::Token> const& input) const
        {
            printStates();
            printTokens();
            printInput(input);

            debug << std::endl;
        }

        void printInput(std::deque<Token::Token> const& input) const
        {
            auto copy = input;
            debug << "Input: ";

            while (!copy.empty())
            {
                debug << Token::tokenTypeToString(copy.front().type) << " ";
                copy.pop_front();
            }

            debug << std::endl;
        }

        void printStates() const
        {
            debug << "States: ";
            auto statesSnapshot = states;
            while (!statesSnapshot.empty())
            {
                debug << "S" << statesSnapshot.top() << " ";
                statesSnapshot.pop();
            }
            debug << std::endl;
        }

        void printTokens() const
        {
            debug << "Tokens: ";
            auto tokensSnapshot = tokens;
            while (!tokensSnapshot.empty())
            {
                debug << Token::tokenTypeToString(tokensSnapshot.top().type) << " ";
                tokensSnapshot.pop();
            }
            debug << std::endl;
        }

    private:
        Rules::Table rulesTable;
        Generator::Table parseTable;

        std::stack<Token::Token> tokens;
        std::stack<size_t> states;
        std::ostream & debug;
    };
}