#pragma once

#include "token.h"

struct ArgumentToken : public Token
{
    unsigned int index;

    ArgumentToken(const unsigned int index);

    virtual std::string toString() const override;
};