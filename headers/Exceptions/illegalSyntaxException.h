#pragma once

#include <stdexcept>

class IllegalSyntaxException : public std::exception
{
private:
    std::string message;

public:
    IllegalSyntaxException(const std::string& expected, const std::string& input);

    const char* what() const throw() override;
};