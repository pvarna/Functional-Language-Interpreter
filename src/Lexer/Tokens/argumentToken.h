#pragma once

#include "token.h"

class ArgumentToken : public Token
{
private:
    unsigned int index;

public:
    ArgumentToken(const unsigned int index);

    virtual std::string toString() const override;
};