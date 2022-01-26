#pragma once

#include <string>
#include <vector>
#include <stack>
#include "Tokens/token.h"

class Lexer
{
private:
    std::string text;
    std::vector<Token*> tokens;
    std::stack<char> brackets;

    void deallocateTokens();
    
    double getNumber(std::string::iterator& it, bool isNegative, bool& isFractional);

    void number(std::string::iterator& it, bool isNegative);
    void dash(std::string::iterator& it);
    void functionName(std::string::iterator& it);
    void openingBracket(char bracket);
    void closingBracket(std::string::iterator& it, char bracket);
    void comma();
    void argument(std::string::iterator& it);

public:
    Lexer(const std::string& text);

    std::vector<Token*> getTokens();
};