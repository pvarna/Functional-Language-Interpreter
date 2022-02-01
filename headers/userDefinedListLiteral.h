#pragma once

#include "literal.h"
#include <list>

struct UserDefinedListLiteral : public Literal
{
    std::list<double> elements;

    UserDefinedListLiteral(const std::list<double>& elements);

    virtual std::string toString() const override;
};