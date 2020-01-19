#include <stdexcept>
#include "ast_builder/ASTBuilder.h"
#include "slr_parser/Parser.h"
#include "ast/NumberAST.h"
#include "ast/BinaryOperatorAST.h"
#include "ast/ExpressionPairAST.h"
#include "ast/VariableDeclarationAST.h"
#include "ast/AssignmentAST.h"
#include "ast/VariableAccessAST.h"
#include "ast/FunctionCallAST.h"
#include "ast/CompareBinaryOperatorAST.h"
#include "ast/IfAST.h"
#include "ast/WhileAST.h"

using namespace SLR;
using namespace AST;

namespace
{
    template<typename T>
    Rules::Action getBinaryOperatorASTReducer(T & stack, BinaryOperatorAST::Type t)
    {
        return [&stack, t](auto const& tokens) {
            if (stack.size() < 2)
            {
                throw std::logic_error("too small stack for binary operator");
            }

            auto right = std::move(stack.top());
            stack.pop();
            auto left = std::move(stack.top());
            stack.pop();

            stack.emplace(new BinaryOperatorAST(std::move(left), std::move(right), t));
        };
    }

    template<typename T>
    Rules::Action getUnaryMinusASTReducer(T & stack)
    {
        return [&stack](auto const& tokens) {
            if (stack.empty())
            {
                throw std::logic_error("too small stack for unary minus");
            }

            auto left = std::make_unique<NumberAST>(Token::Token{Token::NumberLiteral, "0"});

            auto right = std::move(stack.top());
            stack.pop();

            stack.emplace(new BinaryOperatorAST(std::move(left), std::move(right), BinaryOperatorAST::Type::Sub));
        };
    }

    template<typename T>
    Rules::Action getNumberASTReducer(T & stack)
    {
        return [&stack](auto const& tokens) {
            if (tokens.size() != 1)
            {
                throw std::logic_error("invalid tokens for reduce number");
            }

            stack.emplace(new NumberAST(tokens[0]));
        };
    }

    template<typename T>
    Rules::Action getRootReducer(T const& stack)
    {
        return [&stack](auto const& tokens) {
            if (stack.size() != 1)
            {
                throw std::logic_error("invalid stack size in the end");
            }
        };
    }

    template<typename T>
    Rules::Action getExpressionListReducer(T & stack)
    {
        return [&stack](auto const& tokens) {
            if (stack.size() < 2)
            {
                throw std::logic_error("too small stack for expression list");
            }

            auto right = std::move(stack.top());
            stack.pop();
            auto left = std::move(stack.top());
            stack.pop();

            stack.emplace(new ExpressionPairAST(std::move(left), std::move(right)));
        };
    }

    template<typename T, typename S>
    Rules::Action getVariableDeclarationASTReducer(T & stack, S & scope, AST::ValueType v)
    {
        return [&stack, &scope, v](auto const& tokens) {
            if (tokens.size() != 2)
            {
                throw std::logic_error("invalid tokens for reduce vardecl");
            }

            if (scope.find(tokens[0].value) != scope.end())
            {
                throw std::logic_error("variable already defined in current scope: " + Token::tokenDescription(tokens[0]));
            }

            scope[tokens[0].value] = v;
            stack.emplace(new VariableDeclarationAST(v, tokens[0]));
        };
    }

    template<typename T, typename S>
    Rules::Action getAssignmentASTReducer(T & stack, S & scope)
    {
        return [&stack, &scope](auto const& tokens) {
            if (tokens.size() != 3 || stack.empty())
            {
                throw std::logic_error("invalid state for reduce assignment");
            }

            auto const& id = tokens[2];
            if (scope.find(id.value) == scope.end())
            {
                throw std::logic_error("assignment to a nonexistent variable: " + Token::tokenDescription(id));
            }

            auto value = std::move(stack.top());
            stack.pop();
            auto varType = scope[id.value];
            auto argType = value->getResultType();
            if (varType != argType)
            {
                throw std::logic_error(
                        "assignment types do not match: " +
                        AST::valueTypeToString(varType) + " <> " + AST::valueTypeToString(argType) + " " +
                        Token::tokenDescription(id)
                );
            }

            stack.emplace(new AssignmentAST(id, std::move(value)));
        };
    }

    template<typename T, typename S>
    Rules::Action getVariableAccessASTReducer(T & stack, S & scope)
    {
        return [&stack, &scope](auto const& tokens) {
            if (tokens.size() != 1)
            {
                throw std::logic_error("invalid tokens for reduce variable access");
            }
            auto const& id = tokens[0];
            if (scope.find(id.value) == scope.end())
            {
                throw std::logic_error("trying access to nonexistent variable: " + Token::tokenDescription(id));
            }

            stack.emplace(new VariableAccessAST(id, scope[id.value]));
        };
    }

    template<typename T>
    Rules::Action getFunctionCallASTReducer(T & stack)
    {
        // only print function supported with 1 argument
        return [&stack](auto const& tokens) {
            if (tokens.size() != 4 || stack.empty() || tokens[3].value != "print")
            {
                throw std::logic_error("invalid state for reduce function call");
            }

            auto arg = std::move(stack.top());
            stack.pop();

            stack.emplace(new FunctionCallAST(std::move(arg)));
        };
    }

    template<typename T>
    Rules::Action getCompareBinaryOperatorASTReducer(T & stack, CompareBinaryOperatorAST::Type t)
    {
        return [&stack, t](auto const& tokens) {
            if (stack.size() < 2)
            {
                throw std::logic_error("too small stack for compare binary operator");
            }

            auto right = std::move(stack.top());
            stack.pop();
            auto left = std::move(stack.top());
            stack.pop();

            stack.emplace(new CompareBinaryOperatorAST(std::move(left), std::move(right), t));
        };
    }

    template<typename T>
    Rules::Action getIfASTReducer(T & stack)
    {
        return [&stack](auto const& tokens) {
            if (stack.size() < 2)
            {
                throw std::logic_error("too small stack for if operator");
            }

            auto stmt = std::move(stack.top());
            stack.pop();
            auto cond = std::move(stack.top());
            stack.pop();

            stack.emplace(new IfAST(std::move(cond), std::move(stmt)));
        };
    }

    template<typename T>
    Rules::Action getWhileASTReducer(T & stack)
    {
        return [&stack](auto const& tokens) {
            if (stack.size() < 2)
            {
                throw std::logic_error("too small stack for while operator");
            }

            auto stmt = std::move(stack.top());
            stack.pop();
            auto cond = std::move(stack.top());
            stack.pop();

            stack.emplace(new WhileAST(std::move(cond), std::move(stmt)));
        };
    }
}

Rules::Table ASTBuilder::getRules()
{
    return  {
            {Token::Root,            {Token::StatementList},                                                                   getRootReducer(mStack)},

            {Token::StatementListWhile, {Token::StatementList}},
            {Token::StatementListIf, {Token::StatementList}},
            {Token::StatementList,   {Token::StatementList, Token::Semicolon, Token::Statement},                               getExpressionListReducer(mStack)},
            {Token::StatementList,   {Token::Statement}},

            {Token::Statement,         {Token::Number,          Token::Id},                                                                 getVariableDeclarationASTReducer(mStack, mVariables.top(), AST::ValueType::Number)},
            {Token::Statement,         {Token::String,          Token::Id},                                                                 getVariableDeclarationASTReducer(mStack, mVariables.top(), AST::ValueType::String)},
            {Token::Statement,         {Token::Id,              Token::Equals,          Token::Expression},                                 getAssignmentASTReducer(mStack, mVariables.top())},
            {Token::Statement,         {Token::Id,              Token::OpenParenthesis, Token::Expression,        Token::CloseParenthesis}, getFunctionCallASTReducer(mStack)},
            {Token::Statement,         {Token::If,              Token::OpenParenthesis, Token::CompareExpression, Token::CloseParenthesis,
                                     Token::OpenBrace,   Token::StatementListIf, Token::CloseBrace},                                        getIfASTReducer(mStack)},
            {Token::Statement,         {Token::While,           Token::OpenParenthesis, Token::CompareExpression, Token::CloseParenthesis,
                                             Token::OpenBrace,   Token::StatementListWhile, Token::CloseBrace},                             getWhileASTReducer(mStack)},

            {Token::CompareExpression, {Token::Expression,      Token::Less,            Token::Expression}, getCompareBinaryOperatorASTReducer(
                    mStack, CompareBinaryOperatorAST::Type::Less)},
            {Token::CompareExpression, {Token::Expression,      Token::More,            Token::Expression},                                 getCompareBinaryOperatorASTReducer(
                    mStack, CompareBinaryOperatorAST::Type::More)},
            {Token::CompareExpression, {Token::Expression,      Token::DoubleEquals,    Token::Expression},                                 getCompareBinaryOperatorASTReducer(
                    mStack, CompareBinaryOperatorAST::Type::Equals)},

            {Token::Expression,        {Token::Expression,      Token::Plus,            Token::Expression1},                                getBinaryOperatorASTReducer(mStack, BinaryOperatorAST::Type::Sum)},
            {Token::Expression,        {Token::Expression,      Token::Minus,           Token::Expression1},                                getBinaryOperatorASTReducer(mStack, BinaryOperatorAST::Type::Sub)},
            {Token::Expression,        {Token::Expression1}},
            {Token::Expression1,       {Token::Expression1,     Token::Mult,            Token::Expression2},                                getBinaryOperatorASTReducer(mStack, BinaryOperatorAST::Type::Mul)},
            {Token::Expression1,       {Token::Expression1,     Token::Div,             Token::Expression2},                                getBinaryOperatorASTReducer(mStack, BinaryOperatorAST::Type::Div)},
            {Token::Expression1,       {Token::Expression2}},

            {Token::Expression2,       {Token::OpenParenthesis, Token::Expression,      Token::CloseParenthesis}},
            {Token::Expression2,       {Token::Minus,           Token::Expression2},                                                        getUnaryMinusASTReducer(mStack)},
            {Token::Expression2,   {Token::NumberLiteral}, getNumberASTReducer(mStack)},
            {Token::Expression2,   {Token::Id}, getVariableAccessASTReducer(mStack,  mVariables.top())}
    };
}

std::unique_ptr<IAST> ASTBuilder::getAST()
{
    if (mStack.size() != 1)
    {
        throw std::logic_error("ast not built");
    }

    auto ast = std::move(mStack.top());
    mStack.pop();
    return std::move(ast);
}

std::unique_ptr<IAST> ASTBuilder::build(std::deque<Token::Token> & tokens)
{
    Parser(getRules(), mDebug).parse(tokens);
    return getAST();
}

ASTBuilder::ASTBuilder(std::ostream & debug)
    : mDebug(debug)
{
    mVariables.emplace();
}