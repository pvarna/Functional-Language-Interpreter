#include <iostream>
#include <cstring>
#include "lexer.h"
#include "illegalCharacterException.h"
#include "parser.h"

int main ()
{
    std::string str;

    std::getline(std::cin, str);

    Lexer lexer(str);

    std::vector<Token*> tokens;

    try
    {
        tokens = lexer.getTokens();
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    
    for (Token* token : tokens)
    {
        std::cout << token->toString() << std::endl;
    }

    Parser parser(str, tokens);
    ASTNode* result = parser.parse();

    std::cout << result->toString() << std::endl;

    // for (Token* token : tokens)
    // {
    //     delete token;
    // }

    // tokens.clear();

    return 0;
}