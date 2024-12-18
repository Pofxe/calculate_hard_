#pragma once

#include "Token.h"

class Sorter
{
public:

    Sorter(const std::vector<Token>& tokens_) : tokens(tokens_) {}

    std::vector<Token> GetTokensRPN() const
    {
        return tokens;
    }

    std::vector<Token> GetSortedTokens()
    {
        return Sort();
    }

private:

    std::vector<Token> Sort()
    {
        std::vector<Token> output;
        std::stack<Token> operators;

        for (const Token& token : tokens)
        {
            switch (token.type)
            {
            case TokenType::Number:
            {
                output.push_back(token);
                break;
            }
            case TokenType::Function:
            {
                operators.push(token);
                break;
            }
            case TokenType::Operator:
            {
                    while (!operators.empty() && operators.top().type != TokenType::Brackets && (OperatorPriorities(operators.top().token[0]) >= OperatorPriorities(token.token[0])))
                    {
                        output.push_back(operators.top());
                        operators.pop();
                    }
                    operators.push(token);
                break;
            }
            case TokenType::Brackets:
            {
                if (token.token == "(")
                {
                    operators.push(token);
                }
                else
                {
                    while (!operators.empty() && operators.top().token != "(")
                    {
                        output.push_back(operators.top());
                        operators.pop();
                    }
                    if (!operators.empty() && operators.top().token == "(")
                    {
                        operators.pop();
                    }
                    if (!operators.empty() && operators.top().type == TokenType::Function)
                    {
                        output.push_back(operators.top());
                        operators.pop();
                    }
                }
                break;
            }
            default:
            {
                throw std::runtime_error("Unknown token type: " + token.token);
            }
            }
        }
        while (!operators.empty())
        {
            output.push_back(operators.top());
            operators.pop();
        }
        return output;
    }

    int OperatorPriorities(char op_) const
    {
        static std::unordered_map<char, int> operator_priorities =
        {
            {'+', 1},
            {'-', 1},
            {'*', 2},
            {'/', 2},
            {'%', 2},
            {'~', 3},
            {'^', 4},
            {'!', 5}
        };

        auto it = operator_priorities.find(op_);
        if (it != operator_priorities.end())
        {
            return it->second;
        }

        return -1;
    }

private:

    const std::vector<Token>& tokens;
};