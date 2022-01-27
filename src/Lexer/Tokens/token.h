#pragma once

#include "tokenType.h"
#include <fstream>

class Token
{
private:
    TokenType type;

public:
    Token(const TokenType& type);

    TokenType getType() const;

    virtual std::string toString() const;
};