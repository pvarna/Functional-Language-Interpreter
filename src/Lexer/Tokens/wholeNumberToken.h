#pragma once

#include "token.h"

struct WholeNumberToken : public Token
{
    int value;

    WholeNumberToken(const int value);

    virtual std::string toString() const override;
};