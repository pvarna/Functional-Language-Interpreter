#pragma once

#include "literal.h"

struct WholeNumberLiteral : public Literal
{
    int value;

    WholeNumberLiteral(const int value);

    virtual std::string toString() const override;
    virtual WholeNumberLiteral* clone() const override;

    virtual int length() const override;
    virtual bool toBool() const override;
};