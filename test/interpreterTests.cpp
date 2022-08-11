#include "catch2/catch.hpp"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <iostream>

void clear(std::vector<Token*>& tokens, ASTNode* tree)
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

TEST_CASE("Evaluation of literals")
{
    SECTION("Whole Number")
    {
        std::string number = "5";
        Lexer l(number);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(number, tokens);
        ASTNode* AST = parser.parse();

        Interpreter interpreter;

        REQUIRE(interpreter.interpret(AST) == "5");
        clear(tokens, AST);
    }

    SECTION("Fractional Number")
    {
        std::string number = "2.8";
        Lexer l(number);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(number, tokens);
        ASTNode* AST = parser.parse();

        Interpreter interpreter;

        REQUIRE(interpreter.interpret(AST) == "2.8");
        clear(tokens, AST);
    }

    SECTION("List")
    {
        std::string list = "[ 1 2 3 4 5 ]";
        Lexer l(list);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(list, tokens);
        ASTNode* AST = parser.parse();

        Interpreter interpreter;

        REQUIRE(interpreter.interpret(AST) == "[1 2 3 4 5]");
        clear(tokens, AST);
    }
}

TEST_CASE("Evaluation of function definition")
{
    std::string funcDeclaration = "myList -> [1 2 3 4]";
    Lexer l(funcDeclaration);

    std::vector<Token*> tokens = l.tokenize();

    Parser parser(funcDeclaration, tokens);
    ASTNode* AST = parser.parse();

        Interpreter interpreter;

        REQUIRE(interpreter.interpret(AST) == "0");
        clear(tokens, AST);
}

TEST_CASE("Evaliation of some basic functions")
{
    SECTION("add")
    {
        std::string add = "add(5,6)";
        Lexer l(add);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(add, tokens);
        ASTNode* AST = parser.parse();

        Interpreter interpreter;

        REQUIRE(interpreter.interpret(AST) == "11");
        clear(tokens, AST);
    }

    SECTION("tail")
    {
        std::string tail = "tail([1 2 3 4 5])";
        Lexer l(tail);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(tail, tokens);
        ASTNode* AST = parser.parse();

        Interpreter interpreter;

        REQUIRE(interpreter.interpret(AST) == "[2 3 4 5]");
        clear(tokens, AST);
    }

    SECTION("list")
    {
        std::string list = "list(1, 2, 3)";
        Lexer l(list);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(list, tokens);
        ASTNode* AST = parser.parse();

        Interpreter interpreter;

        REQUIRE(interpreter.interpret(AST) == "[1 3 5]");
        clear(tokens, AST);
    }

    SECTION("eq")
    {
        std::string eq = "eq(5,[5])";
        Lexer l(eq);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(eq, tokens);
        ASTNode* AST = parser.parse();

        Interpreter interpreter;

        REQUIRE(interpreter.interpret(AST) == "1");
        clear(tokens, AST);
    }
}

TEST_CASE("Evaliation of functions composition")
{
    SECTION("Compostiton 1")
    {
        std::string comp = "add(mul(5,6),sub(6,2))";
        Lexer l(comp);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(comp, tokens);
        ASTNode* AST = parser.parse();

        Interpreter interpreter;

        REQUIRE(interpreter.interpret(AST) == "34");
        clear(tokens, AST);
    }

    SECTION("Compostiton 2")
    {
        std::string comp = "list(head([1 2 3]), add(2,3), head(tail(list(1,2,3))))";
        Lexer l(comp);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(comp, tokens);
        ASTNode* AST = parser.parse();

        Interpreter interpreter;

        REQUIRE(interpreter.interpret(AST) == "[1 6 11]");
        clear(tokens, AST);
    }
}

TEST_CASE("Use of user defined function")
{
    std::string funcDef = "double -> add(#0,#0)";
    Lexer l1(funcDef);

    std::vector<Token*> tokens1 = l1.tokenize();

    Parser parser1(funcDef, tokens1);
    ASTNode* AST1 = parser1.parse();

    Interpreter interpreter;

    REQUIRE(interpreter.interpret(AST1) == "0");
    clear(tokens1, AST1);

    std::string func = "double(6)";
    Lexer l2(func);

    std::vector<Token*> tokens2 = l2.tokenize();

    Parser parser2(func, tokens2);
    ASTNode* AST2 = parser2.parse();

    REQUIRE(interpreter.interpret(AST2) == "12");
    clear(tokens2, AST2);
}

TEST_CASE("Recursive functions")
{
    SECTION("Factoriel")
    {
        std::string funcDef = "fact -> if(eq(#0,0),1,mul(#0,fact(sub(#0,1))))";
        Lexer l1(funcDef);

        std::vector<Token*> tokens1 = l1.tokenize();

        Parser parser1(funcDef, tokens1);
        ASTNode* AST1 = parser1.parse();

        Interpreter interpreter;

        REQUIRE(interpreter.interpret(AST1) == "0");
        clear(tokens1, AST1);

        std::string func = "fact(5)";
        Lexer l2(func);

        std::vector<Token*> tokens2 = l2.tokenize();

        Parser parser2(func, tokens2);
        ASTNode* AST2 = parser2.parse();

        REQUIRE(interpreter.interpret(AST2) == "120");
        clear(tokens2, AST2);
    }

    // Credit: Lyuben Georgiev
    SECTION("Sorting")
    {
        std::string func1Def = "min -> if(length(#0), if(nand(nand(length(#1), le(head(#0), head(#1))), 1), min(tail(#0), concat([head(#0)], #1)), min(tail(#0), concat(#1, [head(#0)]))), #1)";
        Lexer l1(func1Def);

        std::vector<Token*> tokens1 = l1.tokenize();

        Parser parser1(func1Def, tokens1);
        ASTNode* AST1 = parser1.parse();

        Interpreter interpreter;

        REQUIRE(interpreter.interpret(AST1) == "0");
        clear(tokens1, AST1);

        std::string func2Def = "sort -> if(length(#0), concat([head(min(#0, []))], sort(tail(min(#0, [])))), [])";
        Lexer l2(func2Def);

        std::vector<Token*> tokens2 = l2.tokenize();

        Parser parser2(func2Def, tokens2);
        ASTNode* AST2 = parser2.parse();

        REQUIRE(interpreter.interpret(AST2) == "0");
        clear(tokens2, AST2);

        std::string func = "sort([5 8 -2 3 1 7 3 10 9 12 18 4])";
        Lexer l3(func);

        std::vector<Token*> tokens3 = l3.tokenize();

        Parser parser3(func, tokens3);
        ASTNode* AST3 = parser3.parse();

        REQUIRE(interpreter.interpret(AST3) == "[-2 1 3 3 4 5 7 8 9 10 12 18]");
        clear(tokens3, AST3);
    }
}

TEST_CASE("Some runtime errors")
{
    SECTION("Division by 0")
    {
        std::string f = "div(5,0)";
        Lexer l(f);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(f, tokens);
        ASTNode* AST = parser.parse();

        Interpreter interpreter;

        REQUIRE_THROWS(interpreter.interpret(AST));
        clear(tokens, AST);
    }
    SECTION("Concatenation of infinite lists")
    {
        std::string f = "concat(list(1),list(2))";
        Lexer l(f);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(f, tokens);
        ASTNode* AST = parser.parse();

        Interpreter interpreter;

        REQUIRE_THROWS(interpreter.interpret(AST));
        clear(tokens, AST);
    }
}