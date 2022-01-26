#pragma once

#include <stdexcept>

class IllegalCharacterException : public std::exception
{
private:
    std::string message;

public:
    IllegalCharacterException(const char ch, const std::string& input, const std::size_t index);

    const char* what() const throw() override;
};