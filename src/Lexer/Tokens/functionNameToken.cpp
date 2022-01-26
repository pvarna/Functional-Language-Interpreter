#include "functionNameToken.h"

FunctionNameToken::FunctionNameToken(const std::string& name) : Token(TokenType::FUNCTION_NAME)
{
    this->name = name;
}

std::string FunctionNameToken::toString() const
{
    std::string result = Token::toString();
    result += ": ";
    result += this->name;
    return result;
}