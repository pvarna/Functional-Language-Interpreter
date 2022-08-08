#pragma once

#include "literal.h"

struct InfiniteListLiteral : public Literal
{
    static const int COUNT_OF_ELEMENTS_TO_BE_PRINTED = 5;

    double firstElement;
    double step;

    InfiniteListLiteral(const double firstElement, const double step = 1);

    virtual std::string toString() const override;
    virtual InfiniteListLiteral* clone() const override;
};