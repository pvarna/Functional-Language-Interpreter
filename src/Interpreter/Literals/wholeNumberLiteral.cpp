#include "wholeNumberLiteral.h"

WholeNumberLiteral::WholeNumberLiteral(const int value)
    : Literal(LiteralType::WHOLE_NUMBER)
{
    this->value = value;
}

std::string WholeNumberLiteral::toString() const
{
    return StringConverter::intToString(this->value);
}