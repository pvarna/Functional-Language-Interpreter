#pragma once

#include "tokenType.h"
#include <fstream>

struct Token
{
    TokenType type;

    Token(const TokenType& type);

    virtual std::string toString() const;
};