#include <cmath>
#include <iostream>
#include "lexer.h"
#include "illegalCharacterException.h"
#include "missingCharacterException.h"
#include "fractionalNumberToken.h"
#include "wholeNumberToken.h"
#include "functionNameToken.h"
#include "argumentToken.h"

const double EPS = 0.00001;

Lexer::Lexer(const std::string& text)
{
    this->text = text;
}

void Lexer::deallocateTokens()
{
    std::size_t size = this->tokens.size();

    for (Token* current : this->tokens) 
    {
        delete current;
    }

    this->tokens.clear();
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
        result /= std::pow(10.0, posAfterDecimalDot);
    }

    if (isFractional && decimalPart - 0 < EPS)
    {
        isFractional = false;
    }

    if (isNegative)
    {
        result *= (-1);
    }

    return result;
}

void Lexer::number(std::string::iterator& it, bool isNegative)
{
    double value;
    bool isFractional;
    try
    {
        value = this->getNumber(it, isNegative, isFractional);
        if (isFractional)
        {
            this->tokens.push_back(new FractionalNumberToken(value));
        }
        else
        {
            this->tokens.push_back(new WholeNumberToken(value));
        }
    }
    catch(const std::exception& e)
    {
        this->deallocateTokens();
        throw;
    }

    if (it != this->text.end() && !isspace(*it) && (*it != ',') && (*it != ')') && (*it != ']')) 
    {
        this->deallocateTokens();
        throw IllegalCharacterException(*it, this->text, it - this->text.begin());
    }
    --it;
}

void Lexer::functionName(std::string::iterator& it)
{
    std::string functionName;

    while (it != this->text.end() && (isalpha(*it) || isdigit(*it)))
    {
        functionName += *it;
        ++it;
    }

    try
    {
        this->tokens.push_back(new FunctionNameToken(functionName));
    }
    catch(const std::bad_alloc& ba)
    {
        this->deallocateTokens();
        throw;
    }

    if (it != this->text.end() &&!isspace(*it) && (*it != '(')) 
    {
        this->deallocateTokens();
        throw IllegalCharacterException(*it, this->text, it - this->text.begin());
    }
    --it;
}

void Lexer::dash(std::string::iterator& it)
{
    ++it;
    if (it == this->text.end())
    {
        this->deallocateTokens();
        throw MissingCharacterException('>', this->text, it - this->text.begin());
    }
    if (*it != '>' && !isdigit(*it))
    {
        this->deallocateTokens();
        throw IllegalCharacterException(*it, this->text, it - this->text.begin());
    }
    if (*it == '>')
    {
        try
        {
            this->tokens.push_back(new Token(TokenType::ARROW));
        }
        catch(const std::bad_alloc& ba)
        {
            this->deallocateTokens();
            throw;
        }
    }
    else if (isdigit(*it))
    {
        this->number(it, true);
    }
}

void Lexer::openingBracket(char bracket)
{
    this->brackets.push(bracket);

    TokenType type = (bracket == '(') ? TokenType::OPENING_ROUND_BRACKET :
                                   TokenType::OPENING_SQUARE_BRACKET;

    try
    {
        this->tokens.push_back(new Token(type));
    }
    catch(const std::bad_alloc& ba)
    {
        this->deallocateTokens();
        throw;
    }
}

void Lexer::closingBracket(std::string::iterator& it, char bracket)
{
    char bracketToBeOnTop = (bracket == ')') ? '(' : '[';

    if (this->brackets.empty() || this->brackets.top() != bracketToBeOnTop)
    {
        this->deallocateTokens();
        throw IllegalCharacterException(*it, this->text, it - this->text.begin());
    }

    this->brackets.pop();

    TokenType type = (bracket == ')') ? TokenType::CLOSING_ROUND_BRACKET :
                                   TokenType::CLOSING_SQUARE_BRACKET;

    try
    {
        this->tokens.push_back(new Token(type));
    }
    catch(const std::bad_alloc& ba)
    {
        this->deallocateTokens();
        throw;
    }
}

void Lexer::comma()
{
    try
    {
        this->tokens.push_back(new Token(TokenType::COMMA));
    }
    catch(const std::bad_alloc& ba)
    {
        this->deallocateTokens();
        throw;
    }
}

void Lexer::argument(std::string::iterator& it)
{
    ++it;
    if (!isdigit(*it))
    {
        this->deallocateTokens();
        throw IllegalCharacterException(*it, this->text, it - this->text.begin());
    }

    double value = 0;
    try
    {
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
    }
    catch(const std::exception& e)
    {
        this->deallocateTokens();
        throw;
    }

    try
    {
        this->tokens.push_back(new ArgumentToken(value));
    }
    catch(const std::exception& e)
    {
        this->deallocateTokens();
        throw;
    }
    --it;
}

std::vector<Token*> Lexer::getTokens()
{
    for (std::string::iterator it = this->text.begin(); it != this->text.end(); ++it)
    {
        if (isspace(*it)) 
        {
            continue;
        }
        else if (*it == '-')
        {
            this->dash(it);
        }
        else if (isdigit(*it))
        {
            this->number(it, false);
        }
        else if (isalpha(*it))
        {
            this->functionName(it);
        }
        else if (*it == '(' || *it == '[')
        {
            this->openingBracket(*it);
        }
        else if (*it == ')' || *it == ']')
        {
            this->closingBracket(it, *it);
        }
        else if (*it == ',')
        {
            this->comma();
        }
        else if (*it == '#')
        {
            this->argument(it);
        }
        else
        {
            this->deallocateTokens();
            throw IllegalCharacterException(*it, this->text, it - this->text.begin());
        }
    }

    if (!this->brackets.empty())
    {
        this->deallocateTokens();
        char expected = this->brackets.top() == '(' ? ')' : ']';
        throw MissingCharacterException(expected, this->text, this->text.end() - this->text.begin());
    }

    return tokens;
}