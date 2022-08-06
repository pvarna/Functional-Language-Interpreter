#include "functionNameToken.h"

FunctionNameToken::FunctionNameToken(const std::string& name) 
    : Token(TokenType::FUNCTION_NAME), name(name) {}

std::string FunctionNameToken::toString() const
{
    std::string result = Token::toString();
    result += ": ";
    result += this->name;
    return result;
}

Token* FunctionNameToken::clone() const
{
    return new FunctionNameToken(this->name);
}