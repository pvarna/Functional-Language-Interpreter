#pragma once

#include <stdexcept>

class MissingCharacterException : public std::exception
{
private:
    std::string message;

public:
    MissingCharacterException(const char ch, const std::string& input, const std::size_t index);

    const char* what() const throw() override;
};