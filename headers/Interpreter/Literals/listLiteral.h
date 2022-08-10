#pragma once

#include "literal.h"

class ListLiteral : public Literal
{
public:
    ListLiteral(const LiteralType& type);

    virtual ListLiteral* clone() const override = 0;

    virtual Literal* head() const = 0;
    virtual ListLiteral* tail() const = 0; 
    virtual bool isInfinite() const = 0;
};