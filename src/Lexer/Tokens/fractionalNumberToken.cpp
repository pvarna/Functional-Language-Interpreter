#include "fractionalNumberToken.h"
#include "stringConverter.h"

FractionalNumberToken::FractionalNumberToken(const double value) 
    : Token(TokenType::FRACTIONAL_NUMBER), value(value) {}

std::string FractionalNumberToken::toString() const
{
    std::string result = Token::toString();
    result += ": ";
    result += StringConverter::doubleToString(this->value);
    return result;
}

Token* FractionalNumberToken::clone() const
{
    return new FractionalNumberToken(this->value);
}