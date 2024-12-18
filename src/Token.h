#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <algorithm>

namespace
{
    bool CheckBrackets(const std::string& expression_)
    {
        std::stack<char> bracket_stack;

        for (char ch : expression_)
        {
            if (ch == '(')
            {
                bracket_stack.push(ch);
            }
            else if (ch == ')')
            {
                if (bracket_stack.empty())
                {
                    return false;
                }
                bracket_stack.pop();
            }
        }
        return bracket_stack.empty();
    }

    std::string BlanksRemoval(const std::string& expression_)
    {
        std::string new_expression = expression_;
        new_expression.erase(std::remove(new_expression.begin(), new_expression.end(), ' '), new_expression.end());
        return new_expression;
    }

    bool IsOperator(const char symbol_)
    {
        const std::string operators = "+-*%/!^";
        return operators.find(symbol_) != std::string::npos;
    }

    bool IsFunction(const std::string& func_)
    {
        std::unordered_set<std::string> functions =
        {
            "sin", "cos", "tan",
            "asin", "acos", "atan",
            "log", "log2", "log10",
            "sqrt", "exp", "abs"
        };
        return functions.find(func_) != functions.end();
    }

    bool IsBrackets(const char symbol_)
    {
        return symbol_ == '(' || symbol_ == ')';
    }

    bool IsDigitOrDot(char symbol_)
    {
        return std::isdigit(symbol_) || symbol_ == '.';
    }

    bool IsConstant(const std::string& constant_)
    {
        std::unordered_set<std::string> constants = { "PI", "E", "PHI" };
        return constants.find(constant_) != constants.end();
    }


} // end namespace

enum class TokenType
{
    Number, // целые, с плавающей точкой и константные числа
    Operator, // + - * % / ! ^
    Function, // sin cos tan asin acos atan sqrt log log2 log10 exp abs
    Brackets, // ( )
    Undefined // неопределенный токен
};

enum class OperatorAssociativity
{
    None,
    Left, // + - * /
    Right // ^
};

struct Token
{
    TokenType type = TokenType::Undefined;
    OperatorAssociativity associativity = OperatorAssociativity::None;
    std::string token = "";
};

class Tokenize
{
public:

    Tokenize(std::string& expression_) : expression(expression_)
    {
        BreakingIntoTokens();
    }

    std::vector<Token> GetTokens() const
    {
        return tokens;
    }

    std::string GetExpression() const
    {
        return expression;
    }

private:

    void BreakingIntoTokens()
    {
        std::string current_token = "";

        for (size_t i = 0; i < expression.size(); ++i)
        {
            char symbol = expression[i];

            if (IsDigitOrDot(symbol) || IsConstant(current_token + symbol))
            {
                current_token += symbol;
            }
            else if (IsOperator(symbol))
            {
                ProcessOperator(symbol, current_token, i);
            }
            else if (IsBrackets(symbol))
            {
                ProcessBracket(symbol, current_token);
            }
            else if (std::isalnum(symbol))
            {
                ProcessFunction(symbol, current_token, i);
            }
            else
            {
                throw std::runtime_error("Unknown character in expression: " + std::string(1, symbol));
            }
        }

        AddRemainingToken(current_token);

        if (!current_token.empty())
        {
            throw std::runtime_error("Unknown token in expression: " + current_token);
        }
    }

    void ProcessOperator(char symbol_, std::string& current_token_, size_t i)
    {
        if (!current_token_.empty())
        {
            AddNumberToken(current_token_);
        }

        if (symbol_ == '-' && (i == 0 || expression[i - 1] == '(' || IsOperator(expression[i - 1])))
        {
            AddUnaryMinusToken();
        }
        else
        {
            AddOperatorToken(symbol_);
        }
    }

    void ProcessBracket(char symbol_, std::string& current_token_)
    {
        if (!current_token_.empty())
        {
            AddNumberToken(current_token_);
        }

        AddBracketToken(symbol_);
    }

    void ProcessFunction(char symbol_, std::string& current_token_, size_t i)
    {
        current_token_ += symbol_;

        if (i + 1 < expression.size() && expression[i + 1] == '(')
        {
            if (IsFunction(current_token_))
            {
                AddFunctionToken(current_token_);
                current_token_ = "";
            }
            else if (IsConstant(current_token_))
            {
                AddConstantToken(current_token_);
                current_token_ = "";
            }
            else
            {
                throw std::runtime_error("Unknown function in expression: " + current_token_);
            }
        }
    }

    void AddNumberToken(std::string& current_token_)
    {
        Token token;
        token.type = TokenType::Number;
        token.token = current_token_;
        tokens.push_back(token);
        current_token_ = "";
    }

    void AddUnaryMinusToken()
    {
        Token token;
        token.type = TokenType::Operator;
        token.token = "~";
        token.associativity = AssociativityOperator('~');
        tokens.push_back(token);
    }

    void AddOperatorToken(char symbol_)
    {
        Token token;
        token.type = TokenType::Operator;
        token.token = symbol_;
        token.associativity = AssociativityOperator(symbol_);
        tokens.push_back(token);
    }

    void AddBracketToken(char symbol_)
    {
        Token token;
        token.type = TokenType::Brackets;
        token.token = symbol_;
        tokens.push_back(token);
    }

    void AddFunctionToken(const std::string& current_token_)
    {
        Token token;
        token.type = TokenType::Function;
        token.token = current_token_;
        tokens.push_back(token);
    }

    void AddConstantToken(const std::string& current_token_)
    {
        Token token;
        token.type = TokenType::Number;
        token.token = current_token_;
        tokens.push_back(token);
    }

    void AddRemainingToken(std::string& current_token_)
    {
        if (!current_token_.empty())
        {
            AddNumberToken(current_token_);
        }
    }

    OperatorAssociativity AssociativityOperator(char op_) const
    {
        static std::unordered_map<char, OperatorAssociativity> operator_associativity =
        {
            {'+', OperatorAssociativity::Left},
            {'-', OperatorAssociativity::Left},
            {'*', OperatorAssociativity::Left},
            {'/', OperatorAssociativity::Left},
            {'^', OperatorAssociativity::Right},
            {'!', OperatorAssociativity::Right},
            {'%', OperatorAssociativity::Left},
            {'~', OperatorAssociativity::Right} // используется для унарного отрицания
        };

        auto it = operator_associativity.find(op_);
        if (it != operator_associativity.end())
        {
            return it->second;
        }

        return OperatorAssociativity::None;
    }

private:

    std::vector<Token> tokens = {};
    std::string expression = "";
};

static Tokenize Build(std::string& expression_)
{
    if(!CheckBrackets(expression_))
    {
        throw std::runtime_error("Incorrect arrangement of brackets in the expression : " + expression_);
    }

    std::string new_expression = BlanksRemoval(expression_);
    return Tokenize(new_expression);
}