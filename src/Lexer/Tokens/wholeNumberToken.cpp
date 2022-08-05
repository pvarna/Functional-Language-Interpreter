#include "wholeNumberToken.h"
#include "stringConverter.h"

WholeNumberToken::WholeNumberToken(const int value) 
    : Token(TokenType::WHOLE_NUMBER), value(value) {}

std::string WholeNumberToken::toString() const
{
    std::string result = Token::toString();
    result += ": ";
    result += StringConverter::intToString(this->value);
    return result;
}