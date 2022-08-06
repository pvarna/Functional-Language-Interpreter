#pragma once

#include "tokenType.h"
#include <fstream>

struct Token
{
    TokenType type;

    Token(const TokenType& type);

    virtual std::string toString() const;
    virtual Token* clone() const;
    
    virtual ~Token() = default;
};