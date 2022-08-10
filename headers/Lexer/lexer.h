/** @file lexer.h
 *  @brief A class for splitting the input into tokens
 *  
 *  @author Peter Kolev
 */

#pragma once

#include <string>
#include <vector>
#include <stack>
#include "token.h"

/**
 * @class Lexer
 */
class Lexer
{
private:
    std::string text; //!< stores the input that should be tokenized

    //! Helper function for deallocating the allocated tokens if an exception occurres
    void deallocateTokens(std::vector<Token*>& tokens); 
    
    //! Helper function for getting a number from a given position
    double getNumber(std::string::iterator& it, bool isNegative, bool& isFractional);

    //! Helper function for allocating Number Token
    //! It can be positive, zero or negative
    //! Also it can be whole or fractional
    Token* number(std::string::iterator& it, bool isNegative);

    //! Helper function for allocating a token whose start is a dash 
    //! It is followed by either '>' (which makes an arrow)
    //! or by a number 
    Token* dash(std::string::iterator& it);

    //! Helper function for allocating Function Name Token
    //! It should start with a letter and end with a space
    Token* functionName(std::string::iterator& it);

    //! Helper functions for allocating Bracket Tokens
    Token* openingBracket(char bracket, std::stack<char>& brackets);
    Token* closingBracket(std::string::iterator& it, char bracket, std::stack<char>& brackets);

    //! Helper function for allocating Comma Token
    Token* comma();

    //! Helper function for allocating Argument Token
    //! It should start with '#' followed by positive whole number or zero
    Token* argument(std::string::iterator& it);

public:
    //! Default constructor
    Lexer(const std::string& text);

    //! Main function which splits the input into tokens
    std::vector<Token*> tokenize();
};