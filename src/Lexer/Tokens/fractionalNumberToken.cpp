#include "fractionalNumberToken.h"

FractionalNumberToken::FractionalNumberToken(const double value) : Token(TokenType::FRACTIONAL_NUMBER)
{
    this->value = value;
}

std::string FractionalNumberToken::toString() const
{
    std::string result = Token::toString();
    result += ": ";
    result += std::to_string(this->value);
    while (result.back() == '0')
    {
        result.pop_back();
    }
    return result;
}