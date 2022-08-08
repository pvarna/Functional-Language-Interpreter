#pragma once

#include "literal.h"

struct FiniteListLiteral : public Literal
{
    double firstElement;
    double step;
    int numberOfElements;

    FiniteListLiteral(const double firstElement, const double step, const int numberOfElements);

    virtual std::string toString() const override;
    virtual FiniteListLiteral* clone() const override;
};