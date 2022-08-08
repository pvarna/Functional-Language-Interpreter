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

void ListFunc::run(std::ostream& out)
{
    while (true)
    {
        std::string str;

        out << "listFunc > ";
        std::getline(std::cin, str);

        if (str == "cls")
        {
            out << std::endl;
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
            out << e.what() << std::endl << std::endl;
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
            deallocateResources(tokens);
            out << e.what() << std::endl << std::endl;
            continue;
        }

        try
        {
            out << this->interpreter.interpret(ast, out) << std::endl << std::endl;
        }
        catch(const std::exception& e)
        {
            deallocateResources(tokens, ast);
            out << e.what() << std::endl << std::endl;
            continue;
        }

        deallocateResources(tokens, ast);
    }
}