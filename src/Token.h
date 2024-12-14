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
        std::string new_expression = std::move(expression_);
        new_expression.erase(std::remove(new_expression.begin(), new_expression.end(), ' '), new_expression.end());
        return new_expression;
    }

} // end namespace

enum class TokenType
{
    Number, // целые или числа с плавающей точкой
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
            char ch = expression[i];

            if (std::isdigit(ch) || ch == '.')
            {
                current_token += ch;
            }
            else if (IsOperator(ch))
            {
                if (!current_token.empty())
                {
                    Token token;
                    token.type = TokenType::Number;
                    token.token = current_token;
                    tokens.push_back(token);
                    current_token = "";
                }

                if (ch == '-' && (i == 0 || expression[i + 1] == '(' || IsOperator(expression[i - 1]))) // отдельно обрабатываем унарное отрицание, заменяя токен
                {
                    Token token;
                    token.type = TokenType::Operator;
                    token.token = "~";
                    token.associativity = AssociativityOperator('~');
                    tokens.push_back(token);
                }
                else
                {
                    Token token;
                    token.type = TokenType::Operator;
                    token.token = ch;
                    token.associativity = AssociativityOperator(ch);
                    tokens.push_back(token);
                }
            }
            else if (IsBrackets(ch))
            {
                if (!current_token.empty())
                {
                    Token token;
                    token.type = TokenType::Number;
                    token.token = current_token;
                    tokens.push_back(token);
                    current_token = "";
                }

                Token token;
                token.type = TokenType::Brackets;
                token.token = ch;
                tokens.push_back(token);
            }
            else if (std::isalpha(ch) || std::isdigit(ch))
            {   
                current_token += ch;

                if (i + 1 < expression.size() && expression[i + 1] == '(')
                {
                    Token token;
                    token.type = TokenType::Function;
                    token.token = current_token;
                    tokens.push_back(token);
                    current_token = "";
                }
            }
            else
            {
                throw std::runtime_error("Unknown character in expression: " + std::string(1, ch));
            }
        }

        if (!current_token.empty())
        {
            Token token;
            token.type = TokenType::Number;
            token.token = current_token;
            tokens.push_back(token);
        }
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

    bool IsOperator(const char symbol_) const
    {
        const std::string operators = "+-*%/!^";
        return operators.find(symbol_) != std::string::npos;
    }

    bool IsFunction(const std::string& func_) const
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