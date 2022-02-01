#pragma once

#include "literalType.h"
#include "stringConverter.h"

struct Literal
{
    LiteralType type;

    Literal(const LiteralType& type);

    bool isNumber() const;
    bool isList() const;

    virtual std::string toString() const = 0;
};