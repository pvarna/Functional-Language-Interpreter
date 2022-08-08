#pragma once

#include "literal.h"

struct FractionalNumberLiteral : public Literal
{
    double value;

    FractionalNumberLiteral(const double value);

    virtual std::string toString() const override;
    virtual FractionalNumberLiteral* clone() const override;
};