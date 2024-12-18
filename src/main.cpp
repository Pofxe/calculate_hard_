#include <iostream>
#include "Calculate.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <expression>" << std::endl;
        return 1;
    }

    try
    {
        std::string expression = argv[1];

        Tokenize tokenizer = Build(expression);
        std::vector<Token> tokens = tokenizer.GetTokens();

        Sorter sorter(tokens);
        std::vector<Token> rpn_tokens = sorter.GetSortedTokens();

        CalculateExpr calculator(rpn_tokens);
        double result = calculator.GetResponse();

        std::cout << "Result: " << result << std::endl;
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}