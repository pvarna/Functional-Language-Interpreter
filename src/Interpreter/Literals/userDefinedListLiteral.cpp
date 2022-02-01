#include "userDefinedListLiteral.h"

UserDefinedListLiteral::UserDefinedListLiteral(const std::list<double>& elements)
    : Literal(LiteralType::USER_DEFINED_LIST)
{
    this->elements = elements;
}

std::string UserDefinedListLiteral::toString() const
{
    return StringConverter::listToString(this->elements);
}