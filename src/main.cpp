#include <iostream>
#include "Token.h"

int main()
{
    std::string expression = "3 + 4 * (2 - 1) + -3 ^ 2 ^ 3 + log10(2) + 3.14";
    Tokenize tokenizer = Build(expression);
    std::vector<Token> tokens = tokenizer.GetTokens();

    std::cout << "Expression: " << tokenizer.GetExpression() << std::endl;
    for (const auto& token : tokens)
    {
        std::cout << "Token: " << token.token << ", Type: " << static_cast<int>(token.type) << " Associativity: " << static_cast<int>(token.associativity) << std::endl;
    }
}