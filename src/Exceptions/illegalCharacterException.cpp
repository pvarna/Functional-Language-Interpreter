#include "illegalCharacterException.h"
#include <iostream>

IllegalCharacterException::IllegalCharacterException(const char ch, const std::string& input,
         const std::size_t index) : std::exception()
{
    this->message = input;
    this->message += '\n';
    for (std::size_t i = 0; i < index; ++i)
    {
        this->message += ' ';
    }
    this->message += '^';
    this->message += '\n';
    this->message += "The character \'";
    this->message += ch;
    this->message += "\' is illegal";
}

const char* IllegalCharacterException::what() const throw()
{
    return this->message.c_str();
}