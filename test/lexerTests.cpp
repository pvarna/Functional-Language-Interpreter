#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "../src/Lexer/lexer.h"

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