#include "fractionalNumberLiteral.h"

FractionalNumberLiteral::FractionalNumberLiteral(const double value)
    : Literal(LiteralType::FRACTIONAL_NUMBER)
{
    this->value = value;
}

std::string FractionalNumberLiteral::toString() const
{
    return StringConverter::doubleToString(this->value);
}