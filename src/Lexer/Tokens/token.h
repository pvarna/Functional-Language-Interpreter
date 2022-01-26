#pragma once

#include "tokenType.h"
#include <fstream>

class Token
{
private:
    TokenType type;

public:
    Token(const TokenType& type);

    virtual std::string toString() const;
};