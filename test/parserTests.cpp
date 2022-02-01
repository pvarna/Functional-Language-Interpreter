#include "catch2/catch.hpp"
#include "lexer.h"
#include "parser.h"

TEST_CASE("Check empty AST")
{
    Lexer l("");
    std::vector<Token*> tokens = l.tokenize();

    Parser parser("", tokens);
    REQUIRE(parser.parse() == nullptr);
}

TEST_CASE("Check AST with only one token")
{
    SECTION("Whole Number")
    {
        std::string number = "5";
        Lexer l(number);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(number, tokens);
        REQUIRE(parser.parse()->toString() == "(WHOLE_NUMBER: 5)");
    }
    SECTION("Fractional Number")
    {
        std::string number = "2.7";
        Lexer l(number);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(number, tokens);
        REQUIRE(parser.parse()->toString() == "(FRACTIONAL_NUMBER: 2.7)");
    }
    SECTION("List")
    {
        std::string list = "[1 2 3]";
        Lexer l(list);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(list, tokens);
        REQUIRE(parser.parse()->toString() == "[ (WHOLE_NUMBER: 1) (WHOLE_NUMBER: 2) (WHOLE_NUMBER: 3) ]");
    }
    SECTION("Function name without arguments")
    {
        std::string func = "myFunc()";
        Lexer l(func);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(func, tokens);
        REQUIRE(parser.parse()->toString() == "(FUNCTION_NAME: myFunc)");
    }
    SECTION("Function with two arguments")
    {
        std::string func = "list(1, 2, 3)";
        Lexer l(func);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(func, tokens);
        REQUIRE(parser.parse()->toString() == "(FUNCTION_NAME: list (WHOLE_NUMBER: 1) (WHOLE_NUMBER: 2) (WHOLE_NUMBER: 3))");
    }
    SECTION("Function definition")
    {
        std::string def = "myN -> 8";
        Lexer l(def);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(def, tokens);
        REQUIRE(parser.parse()->toString() == "(FUNCTION_NAME: myN (WHOLE_NUMBER: 8))");
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
        REQUIRE(parser.parse()->toString() == "(FUNCTION_NAME: add (FUNCTION_NAME: sub (FUNCTION_NAME: int (FRACTIONAL_NUMBER: 5.4)) (WHOLE_NUMBER: 2)) (WHOLE_NUMBER: 3))");
    }

    SECTION("Composition #2")
    {
        std::string comp = "head(tail([1 2 3]))";
        Lexer l(comp);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(comp, tokens);
        REQUIRE(parser.parse()->toString() == "(FUNCTION_NAME: head (FUNCTION_NAME: tail [ (WHOLE_NUMBER: 1) (WHOLE_NUMBER: 2) (WHOLE_NUMBER: 3) ]))");
    }

    SECTION("Composition #3")
    {
        std::string comp = "isEven -> nand(isOdd(#0), 1)";
        Lexer l(comp);

        std::vector<Token*> tokens = l.tokenize();

        Parser parser(comp, tokens);
        REQUIRE(parser.parse()->toString() == "(FUNCTION_NAME: isEven (FUNCTION_NAME: nand (FUNCTION_NAME: isOdd (ARGUMENT: 0)) (WHOLE_NUMBER: 1)))");
    }
}

TEST_CASE("Some illegal inputs")
{
    std::string input;
    
    input = "->";
    REQUIRE_THROWS(Parser(input, Lexer(input).tokenize()).parse());

    input = "[,]";
    REQUIRE_THROWS(Parser(input, Lexer(input).tokenize()).parse());

    input = "list(2,3,)";
    REQUIRE_THROWS(Parser(input, Lexer(input).tokenize()).parse());

    input = "[alo]";
    REQUIRE_THROWS(Parser(input, Lexer(input).tokenize()).parse());

    input = "func";
    REQUIRE_THROWS(Parser(input, Lexer(input).tokenize()).parse());

    input = "f[] -> 5";
    REQUIRE_THROWS(Parser(input, Lexer(input).tokenize()).parse());

    input = "5()";
    REQUIRE_THROWS(Parser(input, Lexer(input).tokenize()).parse());
}