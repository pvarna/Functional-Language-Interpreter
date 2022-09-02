#include "listFunc.h"
#include <iostream>

void ListFunc::deallocateResources(std::vector<Token*>& tokens, ASTNode* tree)
{
    for (Token* token : tokens)
    {
        delete token;
    }

    tokens.clear();

    if (tree)
    {
        delete tree;
    }
}

ListFunc& ListFunc::getIstance()
{
    static ListFunc object;

    return object;
}

void ListFunc::run()
{
    while (true)
    {
        std::string str;

        std::cout << "listFunc > ";
        std::getline(std::cin, str);

        if (str == "cls")
        {
            std::cout << std::endl;
            break;
        }

        Lexer lexer(str);
        std::vector<Token*> tokens;

        try
        {
            tokens = lexer.tokenize();
        }
        catch(std::exception& e)
        {
            deallocateResources(tokens);
            std::cout << e.what() << std::endl << std::endl;
            continue;
        }

        Parser parser(str, tokens);
        ASTNode* ast = nullptr;

        try
        {
            ast = parser.parse();
        }
        catch(const std::exception& e)
        {
            deallocateResources(tokens, ast);
            std::cout << e.what() << std::endl << std::endl;
            continue;
        }

        try
        {
            std::cout << this->interpreter.interpret(ast) << std::endl << std::endl;
        }
        catch(const std::exception& e)
        {
            deallocateResources(tokens, ast);
            std::cout << e.what() << std::endl << std::endl;
            continue;
        }

        deallocateResources(tokens, ast);
    }
}