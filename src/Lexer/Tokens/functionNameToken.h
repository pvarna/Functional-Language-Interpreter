#pragma once

#include "token.h"

class FunctionNameToken : public Token
{
private:
    std::string name;

public:
    FunctionNameToken(const std::string& name);

    virtual std::string toString() const override;
};