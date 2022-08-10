#include "fractionalNumberLiteral.h"
#include "stringConverter.h"

FractionalNumberLiteral::FractionalNumberLiteral(const double value)
    : Literal(LiteralType::FRACTIONAL_NUMBER), value(value) {}

std::string FractionalNumberLiteral::toString() const
{
    return StringConverter::doubleToString(this->value);
}

FractionalNumberLiteral* FractionalNumberLiteral::clone() const
{
    return new FractionalNumberLiteral(this->value);
}

int FractionalNumberLiteral::length() const
{
    return -1;
}