#pragma once

#include "literalType.h"
#include <string>

struct Literal
{
    LiteralType type;

    Literal(const LiteralType& type);

    virtual std::string toString() const = 0;
    virtual Literal* clone() const = 0;

    virtual ~Literal() = default;

    virtual int length() const = 0; 

    static Literal* of(const double number);
};