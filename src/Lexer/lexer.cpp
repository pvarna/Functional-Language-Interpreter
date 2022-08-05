#include <cmath>
#include <iostream>
#include "lexer.h"
#include "illegalCharacterException.h"
#include "missingCharacterException.h"
#include "fractionalNumberToken.h"
#include "wholeNumberToken.h"
#include "functionNameToken.h"
#include "argumentToken.h"

const double Lexer::EPS = 0.00001;

Lexer::Lexer(const std::string& text) : text(text) {}

void Lexer::deallocateTokens(std::vector<Token*>& tokens)
{
    for (Token* current : tokens) 
    {
        delete current;
    }

    tokens.clear();
}

double Lexer::getNumber(std::string::iterator& it, bool isNegative, bool& isFractional)
{
    double result = 0;
    double decimalPart = 0;
    double posAfterDecimalDot = 0;
    isFractional = false;

    while (it != this->text.end() && (isdigit(*it) || *it == '.'))
    {    
        if (*it == '.')
        {
            if (isFractional)
            {
                throw IllegalCharacterException(*it, this->text, it - this->text.begin());
            }
            isFractional = true;
        }
        else 
        {
            if (isFractional)
            {
                ++posAfterDecimalDot;
                decimalPart *= 10;
                decimalPart += (*it - '0');
            }
            
            result *= 10;
            result += (*it - '0');
        }

        ++it;
    }

    if (isFractional)
    {
        for (int i = 0; i < posAfterDecimalDot; ++i) 
        {
            result /= 10;
        }
    }

    if (isFractional && std::abs(decimalPart - 0) < EPS)
    {
        isFractional = false;
    }

    if (isNegative)
    {
        result *= (-1);
    }

    return result;
}

Token* Lexer::number(std::string::iterator& it, bool isNegative)
{
    double value;
    bool isFractional;

    Token* result = nullptr;

    value = this->getNumber(it, isNegative, isFractional);

    if (it != this->text.end() && !isspace(*it) && (*it != ',') && (*it != ')') && (*it != ']')) 
    {
        throw IllegalCharacterException(*it, this->text, it - this->text.begin());
    }
    --it;

    if (isFractional)
    {
        return new FractionalNumberToken(value);
    }
    return new WholeNumberToken(value);
}

Token* Lexer::functionName(std::string::iterator& it)
{
    std::string functionName;

    while (it != this->text.end() && (isalpha(*it) || isdigit(*it)))
    {
        functionName += *it;
        ++it;
    }

    if (it != this->text.end() &&!isspace(*it) && (*it != '(')) 
    {
        throw IllegalCharacterException(*it, this->text, it - this->text.begin());
    }
    --it;

    return new FunctionNameToken(functionName);
}

Token* Lexer::dash(std::string::iterator& it)
{
    ++it;
    if (it == this->text.end())
    {
        throw MissingCharacterException('>', this->text, it - this->text.begin());
    }

    if (*it == '>')
    {
        return new Token(TokenType::ARROW);
    }
    else if (isdigit(*it))
    {
        return this->number(it, true);
    }

    throw IllegalCharacterException(*it, this->text, it - this->text.begin());
}

Token* Lexer::openingBracket(char bracket, std::stack<char>& brackets)
{
    brackets.push(bracket);

    TokenType type = (bracket == '(') ? TokenType::OPENING_ROUND_BRACKET :
                                        TokenType::OPENING_SQUARE_BRACKET;

    return new Token(type);
}

Token* Lexer::closingBracket(std::string::iterator& it, char bracket, std::stack<char>& brackets)
{
    char bracketToBeOnTop = (bracket == ')') ? '(' : '[';

    if (brackets.empty() || brackets.top() != bracketToBeOnTop)
    {
        throw IllegalCharacterException(*it, this->text, it - this->text.begin());
    }

    brackets.pop();

    TokenType type = (bracket == ')') ? TokenType::CLOSING_ROUND_BRACKET :
                                        TokenType::CLOSING_SQUARE_BRACKET;

    return new Token(type);
}

Token* Lexer::comma()
{
    return new Token(TokenType::COMMA);
}

Token* Lexer::argument(std::string::iterator& it)
{
    ++it;
    if (!isdigit(*it))
    {
        throw IllegalCharacterException(*it, this->text, it - this->text.begin());
    }

    double value = 0;
    
    while (it != this->text.end() && (isdigit(*it) || *it == '.'))
    {    
        if (*it == '.')
        {
            throw IllegalCharacterException(*it, this->text, it - this->text.begin());
        }
        else 
        {   
            value *= 10;
            value += (*it - '0');
        }
        ++it;
    }

    --it;
    return new ArgumentToken(value);
}

std::vector<Token*> Lexer::tokenize()
{
    std::vector<Token*> tokens;
    std::stack<char> brackets;

    for (std::string::iterator it = this->text.begin(); it != this->text.end(); ++it)
    {
        Token* tokenToAdd = nullptr;

        try 
        {
            if (isspace(*it)) 
            {
                continue;
            }
            else if (*it == '-')
            {
                tokenToAdd = this->dash(it);
            }
            else if (isdigit(*it))
            {
                tokenToAdd = this->number(it, false);
            }
            else if (isalpha(*it))
            {
                tokenToAdd = this->functionName(it);
            }
            else if (*it == '(' || *it == '[')
            {
                tokenToAdd = this->openingBracket(*it, brackets);
            }
            else if (*it == ')' || *it == ']')
            {
                tokenToAdd = this->closingBracket(it, *it, brackets);
            }
            else if (*it == ',')
            {
                tokenToAdd = this->comma();
            }
            else if (*it == '#')
            {
                tokenToAdd = this->argument(it);
            }
            else
            {
                throw IllegalCharacterException(*it, this->text, it - this->text.begin());
            }
        }
        catch(...) 
        {
            this->deallocateTokens(tokens);
            throw;
        }

        if (tokenToAdd) 
        {
            tokens.push_back(tokenToAdd);
        }
    }

    if (!brackets.empty())
    {
        this->deallocateTokens(tokens);
        char expected = brackets.top() == '(' ? ')' : ']';
        throw MissingCharacterException(expected, this->text, this->text.end() - this->text.begin());
    }

    return tokens;
}