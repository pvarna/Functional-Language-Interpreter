#include "literal.h"

Literal::Literal(const LiteralType& type)
{
    this->type = type;
}

bool Literal::isNumber() const
{
    return this->type == LiteralType::FRACTIONAL_NUMBER || this->type == LiteralType::WHOLE_NUMBER;
}

bool Literal::isList() const
{
    return this->type == LiteralType::USER_DEFINED_LIST || this->type == LiteralType::FUNCTION_DEFINED_LIST;
}