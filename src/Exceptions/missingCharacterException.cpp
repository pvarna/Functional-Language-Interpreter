#include "missingCharacterException.h"
#include <iostream>

MissingCharacterException::MissingCharacterException(const char ch, const std::string& input,
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
    this->message += "Expected: \'";
    this->message += ch;
    this->message += "\'";
}

const char* MissingCharacterException::what() const throw()
{
    return this->message.c_str();
}