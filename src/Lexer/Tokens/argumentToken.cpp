#include "argumentToken.h"

ArgumentToken::ArgumentToken(const unsigned int index) : Token(TokenType::ARGUMENT)
{
    this->index = index;
}

std::string ArgumentToken::toString() const
{
    std::string result = Token::toString();
    result += ": ";
    result += std::to_string(this->index);
    return result;
}