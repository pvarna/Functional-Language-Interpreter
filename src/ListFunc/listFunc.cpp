#include "listFunc.h"
#include <iostream>

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
            for (Token* current : tokens)
            {
                delete current;
            }
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
            for (Token* current : tokens)
            {
                delete current;
            }
            delete ast;
            out << e.what() << std::endl << std::endl;
            continue;
        }

        try
        {
            out << this->interpreter.interpret(ast, out) << std::endl << std::endl;
        }
        catch(const std::exception& e)
        {
            out << e.what() << std::endl << std::endl;
        }
    }
}