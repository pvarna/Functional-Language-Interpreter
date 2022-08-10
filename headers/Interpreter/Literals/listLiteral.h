#pragma once

#include "literal.h"

class ListLiteral : public Literal
{
public:
    ListLiteral(const LiteralType& type);

    virtual Literal* head() const = 0;
    virtual ListLiteral* tail() const = 0; 
};