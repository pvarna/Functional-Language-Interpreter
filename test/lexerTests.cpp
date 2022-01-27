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
            std::vector<Token*> tokens = positive.getTokens();
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0]->toString() == "WHOLE_NUMBER: 5");

            deallocateTokens(tokens);
        }

        SECTION("Negative whole number")
        {
            Lexer negative("-3");
            std::vector<Token*> tokens = negative.getTokens();
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0]->toString() == "WHOLE_NUMBER: -3");

            deallocateTokens(tokens);
        }

        SECTION("Zero")
        {
            Lexer zero("0");
            std::vector<Token*> tokens = zero.getTokens();
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0]->toString() == "WHOLE_NUMBER: 0");

            deallocateTokens(tokens);
        }

        SECTION("Number with zero decimal part")
        {
            Lexer zeroDecimal("12.0000");
            std::vector<Token*> tokens = zeroDecimal.getTokens();
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
            std::vector<Token*> tokens = positive.getTokens();
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0]->toString() == "FRACTIONAL_NUMBER: 2.3");

            deallocateTokens(tokens);
        }

        SECTION("Negative fractional number")
        {
            Lexer negative("-3.8");
            std::vector<Token*> tokens = negative.getTokens();
            REQUIRE(tokens.size() == 1);
            REQUIRE(tokens[0]->toString() == "FRACTIONAL_NUMBER: -3.8");

            deallocateTokens(tokens);
        }
    }

    SECTION("Argument")
    {
        Lexer argument("#0");
        std::vector<Token*> tokens = argument.getTokens();
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0]->toString() == "ARGUMENT: 0");

        deallocateTokens(tokens);
    }

    SECTION("Round brackets")
    {
        Lexer roundBrackets("()");
        std::vector<Token*> tokens = roundBrackets.getTokens();
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0]->toString() == "OPENING_ROUND_BRACKET");
        REQUIRE(tokens[1]->toString() == "CLOSING_ROUND_BRACKET");

        deallocateTokens(tokens);
    }

    SECTION("Square brackets")
    {
        Lexer squareBrackets("[]");
        std::vector<Token*> tokens = squareBrackets.getTokens();
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0]->toString() == "OPENING_SQUARE_BRACKET");
        REQUIRE(tokens[1]->toString() == "CLOSING_SQUARE_BRACKET");

        deallocateTokens(tokens);
    }

    SECTION("Arrow")
    {
        Lexer arrow("->");
        std::vector<Token*> tokens = arrow.getTokens();
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0]->toString() == "ARROW");

        deallocateTokens(tokens);
    }

    SECTION("Comma")
    {
        Lexer comma(",");
        std::vector<Token*> tokens = comma.getTokens();
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0]->toString() == "COMMA");

        deallocateTokens(tokens);
    }
}

TEST_CASE("Check some of the examples from the description")
{
    SECTION("Example #1")
    {
        Lexer ex1("isOdd -> eq(mod(int(#0), 2), 1)");
        std::vector<Token*> tokens = ex1.getTokens();
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
        std::vector<Token*> tokens = ex2.getTokens();
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
        REQUIRE_THROWS(Lexer("2 + 3").getTokens());
        REQUIRE_THROWS(Lexer("2 ? 5 : -3").getTokens());
        REQUIRE_THROWS(Lexer("!alo").getTokens());
    }

    SECTION("Arguments")
    {
        REQUIRE_THROWS(Lexer("#2.3").getTokens());
        REQUIRE_THROWS(Lexer("#-2").getTokens());
    }

    SECTION("Brackets")
    {
        REQUIRE_THROWS(Lexer("(").getTokens());
        REQUIRE_THROWS(Lexer("]").getTokens());
        REQUIRE_THROWS(Lexer("((())").getTokens());
        REQUIRE_THROWS(Lexer("[[]]]").getTokens());
        REQUIRE_THROWS(Lexer("(([)]]").getTokens());
    }

    SECTION("Arrow")
    {
        REQUIRE_THROWS(Lexer("<-").getTokens());
        REQUIRE_THROWS(Lexer("<->").getTokens());
    }

    SECTION("Function name starting with digit")
    {
        REQUIRE_THROWS(Lexer("1function").getTokens());
        REQUIRE_THROWS(Lexer("23alo").getTokens());
    }

    SECTION("Numbers")
    {
        REQUIRE_THROWS(Lexer("23.00.0").getTokens());
        REQUIRE_THROWS(Lexer("16f.05").getTokens());
    }
}