#include "argumentToken.h"
#include "stringConverter.h"

ArgumentToken::ArgumentToken(const unsigned int index) 
    : Token(TokenType::ARGUMENT), index(index) {}

std::string ArgumentToken::toString() const
{
    std::string result = Token::toString();
    result += ": ";
    result += StringConverter::intToString(this->index);
    return result;
}