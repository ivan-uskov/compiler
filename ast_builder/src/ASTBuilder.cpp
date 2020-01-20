#include <stdexcept>
#include "ast_builder/ASTBuilder.h"
#include "slr_parser/Parser.h"
#include "ast/IntAST.h"
#include "ast/NumberBinaryOperatorAST.h"
#include "ast/ExpressionPairAST.h"
#include "ast/VariableDeclarationAST.h"
#include "ast/AssignmentAST.h"
#include "ast/VariableAccessAST.h"
#include "ast/FunctionCallAST.h"
#include "ast/CompareBinaryOperatorAST.h"
#include "ast/IfAST.h"
#include "ast/WhileAST.h"
#include "ast/StringAST.h"
#include "ast/DoubleAST.h"
#include "ast/ArrayAssignmentAST.h"
#include "ast/ArrayAccessAST.h"

using namespace SLR;
using namespace AST;

namespace
{
    template<typename T>
    Rules::Action getNumberBinaryOperatorASTReducer(T & stack, NumberBinaryOperatorAST::Type t)
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

            stack.emplace(new NumberBinaryOperatorAST(std::move(left), std::move(right), t));
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

            auto left = std::make_unique<IntAST>(Token::Token{Token::IntLiteral, "0"});

            auto right = std::move(stack.top());
            stack.pop();

            stack.emplace(new NumberBinaryOperatorAST(std::move(left), std::move(right), NumberBinaryOperatorAST::Type::Sub));
        };
    }

    template<typename T>
    Rules::Action getIntASTReducer(T & stack)
    {
        return [&stack](auto const& tokens) {
            if (tokens.size() != 1)
            {
                throw std::logic_error("invalid tokens for reduce int");
            }

            stack.emplace(new IntAST(tokens[0]));
        };
    }

    template<typename T>
    Rules::Action getDoubleASTReducer(T & stack)
    {
        return [&stack](auto const& tokens) {
            if (tokens.size() != 1)
            {
                throw std::logic_error("invalid tokens for reduce double");
            }

            stack.emplace(new DoubleAST(tokens[0]));
        };
    }

    template<typename T>
    Rules::Action getStringASTReducer(T & stack)
    {
        return [&stack](auto const& tokens) {
            if (tokens.size() != 1)
            {
                throw std::logic_error("invalid tokens for reduce string");
            }

            stack.emplace(new StringAST(tokens[0]));
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
            if (!(tokens.size() == 2 || (tokens.size() == 4 && isArrayType(v))))
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
    Rules::Action getArrayAssignmentASTReducer(T & stack, S & scope, bool hasIndex = false)
    {
        return [&stack, &scope, hasIndex](auto const& tokens) {
            if (tokens.size() != (hasIndex ? 5 : 4) || stack.empty())
            {
                throw std::logic_error("invalid state for reduce array assignment");
            }

            auto id = tokens[hasIndex ? 4 : 3];
            if (scope.find(id.value) == scope.end())
            {
                throw std::logic_error("array assignment to a nonexistent variable: " + Token::tokenDescription(id));
            }

            auto value = std::move(stack.top());
            stack.pop();
            auto varType = scope[id.value];
            auto argType = value->getResultType();

            if (!(isArrayType(varType) && getArrayItemType(varType) == argType))
            {
                throw std::logic_error(
                        "array assignment types do not match: " +
                        AST::valueTypeToString(varType) + " <> " + AST::valueTypeToString(argType) + " " +
                        Token::tokenDescription(id)
                );
            }

            if (hasIndex)
            {
                auto index = std::move(stack.top());
                stack.pop();
                if (index->getResultType() != ValueType::Int)
                {
                    throw std::logic_error("invalid type for index: " + valueTypeToString(index->getResultType()));
                }

                stack.emplace(new ArrayAssignmentAST(id, std::move(index), std::move(value)));
            }
            else
            {
                stack.emplace(new ArrayAssignmentAST(id, std::move(value)));
            }
        };
    }

    template<typename T, typename S>
    Rules::Action getArrayAccessASTReducer(T & stack, S & scope, ValueType t)
    {
        return [&stack, &scope, t] (auto const& tokens) {
            if (tokens.size() != 4)
            {
                throw std::logic_error("invalid tokens for reduce array access");
            }
            auto const& id = tokens[3];
            if (!isArrayType(scope[id.value]))
            {
                throw std::logic_error("trying array access to nonarray variable: " + Token::tokenDescription(id));
            }

            auto index = std::move(stack.top());
            stack.pop();
            if (index->getResultType() != ValueType::Int)
            {
                throw std::logic_error("invalid type for index on array access: " + valueTypeToString(index->getResultType()));
            }

            ValueType resType = getArrayItemType(scope[id.value]);
            if (!isTypesMatch(resType, t))
            {
                throw std::logic_error("expected type" + valueTypeToString(t) + " occurred array access: " + Token::tokenDescription(id));
            }

            stack.emplace(new ArrayAccessAST(id, std::move(index), resType));
        };
    }

    template<typename T, typename S>
    Rules::Action getVariableAccessASTReducer(T & stack, S & scope, ValueType t)
    {
        return [&stack, &scope, t](auto const& tokens) {
            if (tokens.size() != 1)
            {
                throw std::logic_error("invalid tokens for reduce variable access");
            }
            auto const& id = tokens[0];
            if (scope.find(id.value) == scope.end())
            {
                throw std::logic_error("trying access to nonexistent variable: " + Token::tokenDescription(id));
            }

            if (!isTypesMatch(scope[id.value], t))
            {
                //throw std::logic_error("expected type" + valueTypeToString(t) + " occurred " + valueTypeToString(scope[id.value]) + " variable: " + Token::tokenDescription(id));
            }

            stack.emplace(new VariableAccessAST(id, scope[id.value]));
        };
    }

    template<typename T>
    Rules::Action getFunctionCallASTReducer(T & stack)
    {
        return [&stack](auto const& tokens) {
            if (tokens.size() != 4 || stack.empty() || !FunctionCallAST::isSupportedFunctionName(tokens[3].value))
            {
                throw std::logic_error("invalid state for reduce function call");
            }

            auto arg = std::move(stack.top());
            stack.pop();

            stack.emplace(new FunctionCallAST(tokens[3].value, std::move(arg)));
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
    return {
            {Token::Root,               {Token::StatementList}, getRootReducer(mStack)},

            {Token::StatementListWhile, {Token::StatementList}},
            {Token::StatementListIf,    {Token::StatementList}},
            {Token::StatementList,      {Token::StatementList, Token::Semicolon, Token::Statement}, getExpressionListReducer(mStack)},
            {Token::StatementList,      {Token::Statement}},

            {Token::Statement,          {Token::Double, Token::Id}, getVariableDeclarationASTReducer(mStack, mVariables.top(), AST::ValueType::Double)},
            {Token::Statement,          {Token::Int, Token::Id}, getVariableDeclarationASTReducer(mStack, mVariables.top(), AST::ValueType::Int)},
            {Token::Statement,          {Token::String, Token::Id}, getVariableDeclarationASTReducer(mStack, mVariables.top(), AST::ValueType::String)},
            {Token::Statement,          {Token::Bool, Token::Id}, getVariableDeclarationASTReducer(mStack, mVariables.top(), AST::ValueType::Bool)},

            {Token::Statement,          {Token::Double, Token::OpenSquareBrace, Token::CloseSquareBrace, Token::Id}, getVariableDeclarationASTReducer(mStack, mVariables.top(), AST::ValueType::DoubleArray)},
            {Token::Statement,          {Token::Int, Token::OpenSquareBrace, Token::CloseSquareBrace, Token::Id}, getVariableDeclarationASTReducer(mStack, mVariables.top(), AST::ValueType::IntArray)},
            {Token::Statement,          {Token::String, Token::OpenSquareBrace, Token::CloseSquareBrace, Token::Id}, getVariableDeclarationASTReducer(mStack, mVariables.top(), AST::ValueType::StringArray)},
            {Token::Statement,          {Token::Bool, Token::OpenSquareBrace, Token::CloseSquareBrace, Token::Id}, getVariableDeclarationASTReducer(mStack, mVariables.top(), AST::ValueType::BoolArray)},

            {Token::Statement,            {Token::Id, Token::Equals, Token::Expression}, getAssignmentASTReducer(mStack, mVariables.top())},

            {Token::Statement,            {Token::IdAndOpenSquareBrace, Token::NumberExpression, Token::CloseSquareBrace, Token::Equals, Token::Expression}, getArrayAssignmentASTReducer(mStack, mVariables.top(), true)},
            {Token::Statement,            {Token::IdAndOpenSquareBrace, Token::CloseSquareBrace, Token::Equals, Token::Expression}, getArrayAssignmentASTReducer(mStack, mVariables.top())},

            {Token::IdAndOpenSquareBrace, {Token::Id, Token::OpenSquareBrace}},

            {Token::Statement,            {Token::Id, Token::OpenParenthesis,  Token::Expression, Token::CloseParenthesis}, getFunctionCallASTReducer(mStack)},

            {Token::Statement,          {Token::If, Token::OpenParenthesis,  Token::BoolExpression, Token::CloseParenthesis,
                                                Token::OpenBrace, Token::StatementListIf,    Token::CloseBrace}, getIfASTReducer(mStack)},
            {Token::Statement,          {Token::While, Token::OpenParenthesis,  Token::BoolExpression, Token::CloseParenthesis,
                                                Token::OpenBrace, Token::StatementListWhile, Token::CloseBrace}, getWhileASTReducer(mStack)},

            {Token::Expression, {Token::NumberExpression}},
            {Token::Expression, {Token::StringExpression}},
            {Token::Expression, {Token::BoolExpression}},

            {Token::StringExpression,   {Token::StringLiteral}, getStringASTReducer(mStack)},
            //{Token::StringExpression,  {Token::Id}, getVariableAccessASTReducer(mStack, mVariables.top(), ValueType::String)},
            //{Token::StringExpression,  {Token::Id, Token::OpenSquareBrace, Token::NumberExpression, Token::CloseSquareBrace}, getArrayAccessASTReducer(mStack, mVariables.top(), ValueType::String)},

            {Token::BoolExpression, {Token::CompareExpression}},
            {Token::BoolExpression, {Token::Id}, getVariableAccessASTReducer(mStack, mVariables.top(), ValueType::Bool)},
            //{Token::BoolExpression, {Token::Id, Token::OpenSquareBrace, Token::NumberExpression, Token::CloseSquareBrace}, getArrayAccessASTReducer(mStack, mVariables.top(), ValueType::Bool)},

            {Token::CompareExpression,  {Token::NumberExpression,  Token::Less, Token::NumberExpression}, getCompareBinaryOperatorASTReducer(mStack, CompareBinaryOperatorAST::Type::Less)},
            {Token::CompareExpression,  {Token::NumberExpression,  Token::More, Token::NumberExpression}, getCompareBinaryOperatorASTReducer(mStack, CompareBinaryOperatorAST::Type::More)},
            {Token::CompareExpression,  {Token::NumberExpression,  Token::DoubleEquals, Token::NumberExpression}, getCompareBinaryOperatorASTReducer(mStack, CompareBinaryOperatorAST::Type::Equals)},

            {Token::NumberExpression,   {Token::NumberExpression,  Token::Plus, Token::NumberExpression1}, getNumberBinaryOperatorASTReducer(mStack, NumberBinaryOperatorAST::Type::Sum)},
            {Token::NumberExpression,   {Token::NumberExpression,  Token::Minus, Token::NumberExpression1}, getNumberBinaryOperatorASTReducer(mStack, NumberBinaryOperatorAST::Type::Sub)},
            {Token::NumberExpression,   {Token::NumberExpression1}},
            {Token::NumberExpression1,  {Token::NumberExpression1, Token::Mult, Token::NumberExpression2}, getNumberBinaryOperatorASTReducer(mStack, NumberBinaryOperatorAST::Type::Mul)},
            {Token::NumberExpression1,  {Token::NumberExpression1, Token::Div, Token::NumberExpression2}, getNumberBinaryOperatorASTReducer(mStack, NumberBinaryOperatorAST::Type::Div)},
            {Token::NumberExpression1,  {Token::NumberExpression2}},

            {Token::NumberExpression2,  {Token::OpenParenthesis, Token::NumberExpression, Token::CloseParenthesis}},
            {Token::NumberExpression2,  {Token::Minus, Token::NumberExpression2}, getUnaryMinusASTReducer(mStack)},
            {Token::NumberExpression2,  {Token::IntLiteral}, getIntASTReducer(mStack)},
            {Token::NumberExpression2,  {Token::DoubleLiteral}, getDoubleASTReducer(mStack)},

            {Token::NumberExpression2,  {Token::Id}, getVariableAccessASTReducer(mStack, mVariables.top(), ValueType::Int)},
            {Token::NumberExpression2,  {Token::NumberExpression3}},

            {Token::NumberExpression3,  {Token::Id, Token::OpenSquareBrace, Token::NumberExpression, Token::CloseSquareBrace}, getArrayAccessASTReducer(mStack, mVariables.top(), ValueType::Int)}
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