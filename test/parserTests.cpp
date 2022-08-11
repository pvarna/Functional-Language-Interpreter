// #define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "lexer.h"
#include "parser.h"
#include <iostream>

void deallocate(std::vector<Token*>& tokens, ASTNode* tree = nullptr)
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

TEST_CASE("Check AST with only one token")
{
    SECTION("Whole Number")
    {
        std::string number = "5";
        Lexer l(number);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(number, tokens);
        ASTNode* AST = parser.parse();
        REQUIRE(AST->toString() == "(WHOLE_NUMBER: 5)");
    
        deallocate(tokens, AST);  
    }
    SECTION("Fractional Number")
    {
        std::string number = "2.7";
        Lexer l(number);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(number, tokens);
        ASTNode* AST = parser.parse();
        REQUIRE(AST->toString() == "(FRACTIONAL_NUMBER: 2.7)");

        deallocate(tokens, AST);
    }
    SECTION("List")
    {
        std::string list = "[1 2 3]";
        Lexer l(list);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(list, tokens);
        ASTNode* AST = parser.parse();
        REQUIRE(AST->toString() == "[ (WHOLE_NUMBER: 1) (WHOLE_NUMBER: 2) (WHOLE_NUMBER: 3) ]");
    
        deallocate(tokens, AST);
    }
    SECTION("Function name without arguments")
    {
        std::string func = "myFunc()";
        Lexer l(func);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(func, tokens);
        ASTNode* AST = parser.parse();
        REQUIRE(AST->toString() == "(FUNCTION_NAME: myFunc)");
    
        deallocate(tokens, AST);
    }
    SECTION("Function with three arguments")
    {
        std::string func = "list(1, 2, 3)";
        Lexer l(func);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(func, tokens);
        ASTNode* AST = parser.parse();
        REQUIRE(AST->toString() == "(FUNCTION_NAME: list (WHOLE_NUMBER: 1) (WHOLE_NUMBER: 2) (WHOLE_NUMBER: 3))");
    
        deallocate(tokens, AST);
    }
    SECTION("Function definition")
    {
        std::string def = "myN -> 8";
        Lexer l(def);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(def, tokens);
        ASTNode* AST = parser.parse();
        REQUIRE(AST->toString() == "(FUNCTION_NAME: myN (WHOLE_NUMBER: 8))");
    
        deallocate(tokens, AST);
    }
    SECTION("Function definition with argument")
    {
        std::string def = "myF -> mul(#0, 1)";
        Lexer l(def);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(def, tokens);
        ASTNode* AST = parser.parse();
        REQUIRE(AST->toString() == "(FUNCTION_NAME: myF (FUNCTION_NAME: mul (ARGUMENT: 0) (WHOLE_NUMBER: 1)))");
    
        deallocate(tokens, AST);
    }
}

TEST_CASE("Compositions")
{
    SECTION("Composition #1")
    {
        std::string comp = "add(sub(int(5.4),2),3)";
        Lexer l(comp);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(comp, tokens);
        ASTNode* AST = parser.parse();
        REQUIRE(AST->toString() == "(FUNCTION_NAME: add (FUNCTION_NAME: sub (FUNCTION_NAME: int (FRACTIONAL_NUMBER: 5.4)) (WHOLE_NUMBER: 2)) (WHOLE_NUMBER: 3))");
    
        deallocate(tokens, AST);    
    }

    SECTION("Composition #2")
    {
        std::string comp = "head(tail([1 2 3]))";
        Lexer l(comp);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(comp, tokens);
        ASTNode* AST = parser.parse();
        REQUIRE(AST->toString() == "(FUNCTION_NAME: head (FUNCTION_NAME: tail [ (WHOLE_NUMBER: 1) (WHOLE_NUMBER: 2) (WHOLE_NUMBER: 3) ]))");
    
        deallocate(tokens, AST);  
    }

    SECTION("Composition #3")
    {
        std::string comp = "isEven -> nand(isOdd(#0), 1)";
        Lexer l(comp);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(comp, tokens);
        ASTNode* AST = parser.parse();
        REQUIRE(AST->toString() == "(FUNCTION_NAME: isEven (FUNCTION_NAME: nand (FUNCTION_NAME: isOdd (ARGUMENT: 0)) (WHOLE_NUMBER: 1)))");
    
        deallocate(tokens, AST);
    }
}

TEST_CASE("Some illegal inputs")
{
    std::string input;
    
    SECTION("Illegal #1")
    {
        input = "->";
        Lexer l(input);

        std::vector<Token*> tokens = l.tokenize();
        
        REQUIRE_THROWS(Parser(input, tokens).parse());
    
        deallocate(tokens);
    }

    SECTION("Illegal #2")
    {
        input = "[,]";
        Lexer l(input);

        std::vector<Token*> tokens = l.tokenize();
        
        REQUIRE_THROWS(Parser(input, tokens).parse());
    
        deallocate(tokens);
    }

    SECTION("Illegal #1")
    {
        input = "list(2,3,)";
        Lexer l(input);

        std::vector<Token*> tokens = l.tokenize();
        
        REQUIRE_THROWS(Parser(input, tokens).parse());
    
        deallocate(tokens);
    }

    SECTION("Illegal #4")
    {
        input = "func";
        Lexer l(input);

        std::vector<Token*> tokens = l.tokenize();
        
        REQUIRE_THROWS(Parser(input, tokens).parse());
    
        deallocate(tokens);
    }

    SECTION("Illegal #5")
    {
        input = "[1, 2, 3, 4,]";
        Lexer l(input);

        std::vector<Token*> tokens = l.tokenize();
        
        REQUIRE_THROWS(Parser(input, tokens).parse());
    
        deallocate(tokens);
    }
}