#include "illegalSyntaxException.h"
#include <iostream>

IllegalSyntaxException::IllegalSyntaxException(const std::string& message, const std::string& input) 
    : std::exception()
{
    this->message = input;
    this->message += '\n';
    this->message += message;
}

const char* IllegalSyntaxException::what() const throw()
{
    return this->message.c_str();
}