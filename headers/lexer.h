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
    std::vector<Token*> tokens; //!< stores the currently allocated tokens
    std::stack<char> brackets; //!< helper stack for checking the correct order of the brackets

    //! Helper function for deallocating the allocated tokens if an exception occurres
    void deallocateTokens(); 
    
    //! Helper function for getting a number from a given position
    double getNumber(std::string::iterator& it, bool isNegative, bool& isFractional);

    //! Helper function for handling the current number - it can be positive, zero or negative
    //! Also it can be whole or fractional
    void number(std::string::iterator& it, bool isNegative);

    //! Helper function for handling the dash - it is followed by either '>' (which makes an arrow)
    //! or by number 
    void dash(std::string::iterator& it);

    //! Helper function for handling function name - it should start with a letter and ends with a space
    void functionName(std::string::iterator& it);

    //! Helper functions for handling brackets
    void openingBracket(char bracket);
    void closingBracket(std::string::iterator& it, char bracket);

    //! Helper function for handling comma
    void comma();

    //! Helper function for handling an argument - it should be '#' followed by positive whole number or zero
    void argument(std::string::iterator& it);

public:
    //! Default constructor
    Lexer(const std::string& text);

    //! Main function which splits the input into tokens
    std::vector<Token*> tokenize();
};