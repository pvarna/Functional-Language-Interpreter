#pragma once

#include "token.h"

class WholeNumberToken : public Token
{
private:
    int value;

public:
    WholeNumberToken(const int value);

    virtual std::string toString() const override;
};