#include "token.h"

Token::Token(const TokenType& type) 
{
    this->type = type;
}

std::string Token::toString() const
{
    switch (this->type)
    {
    case TokenType::OPENING_ROUND_BRACKET:
        return "OPENING_ROUND_BRACKET"; 

    case TokenType::CLOSING_ROUND_BRACKET:
        return "CLOSING_ROUND_BRACKET"; 

    case TokenType::OPENING_SQUARE_BRACKET:
        return "OPENING_SQUARE_BRACKET"; 

    case TokenType::CLOSING_SQUARE_BRACKET:
        return "CLOSING_SQUARE_BRACKET"; 

    case TokenType::FUNCTION_NAME:
        return "FUNCTION_NAME"; 

    case TokenType::ARROW:
        return "ARROW"; 

    case TokenType::COMMA:
        return "COMMA"; 

    case TokenType::ARGUMENT:
        return "ARGUMENT"; 

    case TokenType::WHOLE_NUMBER:
        return "WHOLE_NUMBER"; 

    case TokenType::FRACTIONAL_NUMBER:
        return "FRACTIONAL_NUMBER"; 
    
    default:
        return "UNKNOWN";
    }
}