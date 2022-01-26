#pragma once

#include "token.h"

class FractionalNumberToken : public Token
{
private:
    double value;

public:
    FractionalNumberToken(const double value);

    virtual std::string toString() const override;
};