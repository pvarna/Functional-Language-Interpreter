#pragma once

#include "literal.h"

struct FunctionDefinedListLiteral : public Literal
{
    bool isInfinite;
    double firstElement;
    double step;
    int numberOfElements;

    FunctionDefinedListLiteral(const double firstElement, const double step, const int numberOfElements);

    virtual std::string toString() const override;
};