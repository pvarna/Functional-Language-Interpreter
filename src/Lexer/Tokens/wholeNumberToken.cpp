#include "wholeNumberToken.h"

WholeNumberToken::WholeNumberToken(const int value) : Token(TokenType::WHOLE_NUMBER)
{
    this->value = value;
}

std::string WholeNumberToken::toString() const
{
    std::string result = Token::toString();
    result += ": ";
    result += std::to_string(this->value);
    return result;
}