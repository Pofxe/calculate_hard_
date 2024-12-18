#pragma once

#include "Sorter.h"

#include <cmath>

using Decimal = long double;

static const Decimal PI = 3.141592; // Число Пи
static const Decimal E = 2.718281; // Число Эйлера
static const Decimal PHI = 1.61803398874989484820; // Золотое сечение
static const Decimal SQRTP = 1.41421356237309504880; // Константа Пифагора (корень из 2)
static const Decimal APE = 1.202056903159594285399738161511449990764986292; // Постоянная Апери


class CalculateExpr
{
public:

    CalculateExpr(const std::vector<Token>& rpn_tokens_) : rpn_tokens(rpn_tokens_) {}

    Decimal GetResponse()
    {
        return Evaluate();
    }

private:

    Decimal Evaluate()
    {
        std::stack<Decimal> stack;

        for (const Token& token : rpn_tokens)
        {
            if (token.type == TokenType::Number)
            {
                if (token.token == "PI")
                {
                    stack.push(PI);
                }
                else if (token.token == "E")
                {
                    stack.push(E);
                }
                else if (token.token == "PHI")
                {
                    stack.push(PHI);
                }
                else if (token.token == "SQRTP")
                {
                    stack.push(SQRTP);
                }
                else if (token.token == "APE")
                {
                    stack.push(APE);
                }
                else
                {
                    stack.push(std::stod(token.token));
                }
            }
            else if (token.type == TokenType::Operator)
            {
                if (token.token == "~")
                {
                    if (stack.empty())
                    {
                        throw std::runtime_error("Invalid expression: not enough operands for unary operator.");
                    }
                    Decimal operand = stack.top();
                    stack.pop();
                    stack.push(-operand);
                }
                else if (token.token == "!")
                {
                    if (stack.empty())
                    {
                        throw std::runtime_error("Invalid expression: not enough operands for factorial.");
                    }
                    Decimal operand = stack.top();
                    stack.pop();
                    stack.push(ApplyFactorial(operand));
                }
                else
                {
                    if (stack.size() < 2)
                    {
                        throw std::runtime_error("Invalid expression: not enough operands for binary operator.");
                    }
                    Decimal right = stack.top();
                    stack.pop();
                    Decimal left = stack.top();
                    stack.pop();
                    stack.push(ApplyOperator(left, right, token.token[0]));
                }
            }
            else if (token.type == TokenType::Function)
            {
                if (stack.empty())
                {
                    throw std::runtime_error("Invalid expression: not enough operands for function.");
                }
                Decimal operand = stack.top();
                stack.pop();
                stack.push(ApplyFunction(operand, token.token));
            }
        }

        if (stack.size() != 1)
        {
            throw std::runtime_error("Invalid expression: too many operands.");
        }

        return stack.top();
    }

    Decimal ApplyOperator(Decimal left_, Decimal right_, char op_)
    {
        switch (op_)
        {
        case '+':
        {
            return left_ + right_;
        }
        case '-':
        {
            return left_ - right_;
        }
        case '*':
        {
            return left_ * right_;
        }
        case '/':
        {
            if (right_ == 0)
            {
                throw std::runtime_error("Division by zero");
            }
            return left_ / right_;
        }
        case '^':
        {
            return std::pow(right_, left_);
        }
        case '%':
        {
            return std::fmod(left_, right_);
        }
        default:
        {
            throw std::runtime_error("Unknown operator: " + std::string(1, op_));
        }
        }
    }

    Decimal ApplyFunction(Decimal operand_, const std::string& func_)
    {
        if (func_ == "sin")
        {
            return std::sin(operand_);
        }
        if (func_ == "cos")
        {
            return std::cos(operand_);
        }
        if (func_ == "tan")
        {
            return std::tan(operand_);
        }
        if (func_ == "asin")
        {
            return std::asin(operand_);
        }
        else if (func_ == "acos")
        {
            return std::acos(operand_);
        }
        else if (func_ == "atan")
        {
            return std::atan(operand_);
        }
        else if (func_ == "log")
        {
            return std::log(operand_);
        }
        else if (func_ == "log2")
        {
            return std::log2(operand_);
        }
        else if (func_ == "log10")
        {
            return std::log10(operand_);
        }
        else if (func_ == "sqrt")
        {
            return std::sqrt(operand_);
        }
        else if (func_ == "exp")
        {
            return std::exp(operand_);
        }
        else if (func_ == "abs")
        {
            return std::abs(operand_);
        }
        throw std::runtime_error("Unknown function: " + func_);
    }

    Decimal ApplyFactorial(Decimal operand_)
    {
        if (std::floor(operand_) != operand_ || operand_ < 0)
        {
            throw std::runtime_error("Factorial is defined only for non-negative integers.");
        }

        Decimal result = 1;

        for (int i = 1; i <= static_cast<int>(operand_); ++i)
        {
            result *= i;
        }
        return result;
    }

private:

    const std::vector<Token>& rpn_tokens;
};