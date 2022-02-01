#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "lexer.h"

void deallocateTokens(std::vector<Token*>& tokens)
{
    for (Token* token : tokens)
    {
        delete token;
    }

    tokens.clear();
}

TEST_CASE("Check for valid single tokens")
{
    SECTION("Whole numbers")
    {
        SECTION("Positive whole number")
        {
            Lexer positive("5");
            std::vector<Token*> tokens = positive.tokenize();
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0]->toString() == "WHOLE_NUMBER: 5");

            deallocateTokens(tokens);
        }

        SECTION("Negative whole number")
        {
            Lexer negative("-3");
            std::vector<Token*> tokens = negative.tokenize();
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0]->toString() == "WHOLE_NUMBER: -3");

            deallocateTokens(tokens);
        }

        SECTION("Zero")
        {
            Lexer zero("0");
            std::vector<Token*> tokens = zero.tokenize();
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0]->toString() == "WHOLE_NUMBER: 0");

            deallocateTokens(tokens);
        }

        SECTION("Number with zero decimal part")
        {
            Lexer zeroDecimal("12.0000");
            std::vector<Token*> tokens = zeroDecimal.tokenize();
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0]->toString() == "WHOLE_NUMBER: 12");

            deallocateTokens(tokens);
        }
    }

    SECTION("Fractional numbers")
    {
        SECTION("Positive fractional number")
        {
            Lexer positive("2.3");
            std::vector<Token*> tokens = positive.tokenize();
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0]->toString() == "FRACTIONAL_NUMBER: 2.3");

            deallocateTokens(tokens);
        }

        SECTION("Negative fractional number")
        {
            Lexer negative("-3.8");
            std::vector<Token*> tokens = negative.tokenize();
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0]->toString() == "FRACTIONAL_NUMBER: -3.8");

            deallocateTokens(tokens);
        }
    }

    SECTION("Argument")
    {
        Lexer argument("#0");
        std::vector<Token*> tokens = argument.tokenize();
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0]->toString() == "ARGUMENT: 0");

        deallocateTokens(tokens);
    }

    SECTION("Round brackets")
    {
        Lexer roundBrackets("()");
        std::vector<Token*> tokens = roundBrackets.tokenize();
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0]->toString() == "OPENING_ROUND_BRACKET");
        REQUIRE(tokens[1]->toString() == "CLOSING_ROUND_BRACKET");

        deallocateTokens(tokens);
    }

    SECTION("Square brackets")
    {
        Lexer squareBrackets("[]");
        std::vector<Token*> tokens = squareBrackets.tokenize();
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0]->toString() == "OPENING_SQUARE_BRACKET");
        REQUIRE(tokens[1]->toString() == "CLOSING_SQUARE_BRACKET");

        deallocateTokens(tokens);
    }

    SECTION("Arrow")
    {
        Lexer arrow("->");
        std::vector<Token*> tokens = arrow.tokenize();
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0]->toString() == "ARROW");

        deallocateTokens(tokens);
    }

    SECTION("Comma")
    {
        Lexer comma(",");
        std::vector<Token*> tokens = comma.tokenize();
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0]->toString() == "COMMA");

        deallocateTokens(tokens);
    }
}

TEST_CASE("Check empty input")
{
    Lexer empty("");
    std::vector<Token*> tokens = empty.tokenize();

    REQUIRE(tokens.size() == 0);
}

TEST_CASE("Check some of the examples from the description")
{
    SECTION("Example #1")
    {
        Lexer ex1("isOdd -> eq(mod(int(#0), 2), 1)");
        std::vector<Token*> tokens = ex1.tokenize();
        REQUIRE(tokens.size() == 16);
        REQUIRE(tokens[0]->toString() == "FUNCTION_NAME: isOdd");
        REQUIRE(tokens[1]->toString() == "ARROW");
        REQUIRE(tokens[2]->toString() == "FUNCTION_NAME: eq");
        REQUIRE(tokens[3]->toString() == "OPENING_ROUND_BRACKET");
        REQUIRE(tokens[4]->toString() == "FUNCTION_NAME: mod");
        REQUIRE(tokens[5]->toString() == "OPENING_ROUND_BRACKET");
        REQUIRE(tokens[6]->toString() == "FUNCTION_NAME: int");
        REQUIRE(tokens[7]->toString() == "OPENING_ROUND_BRACKET");
        REQUIRE(tokens[8]->toString() == "ARGUMENT: 0");
        REQUIRE(tokens[9]->toString() == "CLOSING_ROUND_BRACKET");
        REQUIRE(tokens[10]->toString() == "COMMA");
        REQUIRE(tokens[11]->toString() == "WHOLE_NUMBER: 2");
        REQUIRE(tokens[12]->toString() == "CLOSING_ROUND_BRACKET");
        REQUIRE(tokens[13]->toString() == "COMMA");
        REQUIRE(tokens[14]->toString() == "WHOLE_NUMBER: 1");
        REQUIRE(tokens[15]->toString() == "CLOSING_ROUND_BRACKET");
    }

    SECTION("Example #2")
    {
        Lexer ex2("primes10 -> filterPrimes(concat([2], list(3, 1, 7)))");
        std::vector<Token*> tokens = ex2.tokenize();
        REQUIRE(tokens.size() == 20);
        REQUIRE(tokens[0]->toString() == "FUNCTION_NAME: primes10");
        REQUIRE(tokens[1]->toString() == "ARROW");
        REQUIRE(tokens[2]->toString() == "FUNCTION_NAME: filterPrimes");
        REQUIRE(tokens[3]->toString() == "OPENING_ROUND_BRACKET");
        REQUIRE(tokens[4]->toString() == "FUNCTION_NAME: concat");
        REQUIRE(tokens[5]->toString() == "OPENING_ROUND_BRACKET");
        REQUIRE(tokens[6]->toString() == "OPENING_SQUARE_BRACKET");
        REQUIRE(tokens[7]->toString() == "WHOLE_NUMBER: 2");
        REQUIRE(tokens[8]->toString() == "CLOSING_SQUARE_BRACKET");
        REQUIRE(tokens[9]->toString() == "COMMA");
        REQUIRE(tokens[10]->toString() == "FUNCTION_NAME: list");
        REQUIRE(tokens[11]->toString() == "OPENING_ROUND_BRACKET");
        REQUIRE(tokens[12]->toString() == "WHOLE_NUMBER: 3");
        REQUIRE(tokens[13]->toString() == "COMMA");
        REQUIRE(tokens[14]->toString() == "WHOLE_NUMBER: 1");
        REQUIRE(tokens[15]->toString() == "COMMA");
        REQUIRE(tokens[16]->toString() == "WHOLE_NUMBER: 7");
        REQUIRE(tokens[17]->toString() == "CLOSING_ROUND_BRACKET");
        REQUIRE(tokens[18]->toString() == "CLOSING_ROUND_BRACKET");
        REQUIRE(tokens[19]->toString() == "CLOSING_ROUND_BRACKET");
    }
}

TEST_CASE("Some invalid inputs")
{
    SECTION("Invalid characters")
    {
        REQUIRE_THROWS(Lexer("2 + 3").tokenize());
        REQUIRE_THROWS(Lexer("2 ? 5 : -3").tokenize());
        REQUIRE_THROWS(Lexer("!alo").tokenize());
    }

    SECTION("Arguments")
    {
        REQUIRE_THROWS(Lexer("#2.3").tokenize());
        REQUIRE_THROWS(Lexer("#-2").tokenize());
    }

    SECTION("Brackets")
    {
        REQUIRE_THROWS(Lexer("(").tokenize());
        REQUIRE_THROWS(Lexer("]").tokenize());
        REQUIRE_THROWS(Lexer("((())").tokenize());
        REQUIRE_THROWS(Lexer("[[]]]").tokenize());
        REQUIRE_THROWS(Lexer("(([)]]").tokenize());
    }

    SECTION("Arrow")
    {
        REQUIRE_THROWS(Lexer("<-").tokenize());
        REQUIRE_THROWS(Lexer("<->").tokenize());
    }

    SECTION("Function name starting with digit")
    {
        REQUIRE_THROWS(Lexer("1function").tokenize());
        REQUIRE_THROWS(Lexer("23alo").tokenize());
    }

    SECTION("Numbers")
    {
        REQUIRE_THROWS(Lexer("23.00.0").tokenize());
        REQUIRE_THROWS(Lexer("16f.05").tokenize());
    }
}