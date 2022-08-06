#pragma once

#include "token.h"

struct FunctionNameToken : public Token
{
    std::string name;

    FunctionNameToken(const std::string& name);

    virtual std::string toString() const override;
    virtual Token* clone() const override;
};