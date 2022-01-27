#pragma once

#include "token.h"

struct FractionalNumberToken : public Token
{
    double value;

    FractionalNumberToken(const double value);

    virtual std::string toString() const override;
};