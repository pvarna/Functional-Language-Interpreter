#include "wholeNumberLiteral.h"
#include "stringConverter.h"

WholeNumberLiteral::WholeNumberLiteral(const int value)
    : Literal(LiteralType::WHOLE_NUMBER), value(value) {}

std::string WholeNumberLiteral::toString() const
{
    return StringConverter::intToString(this->value);
}

WholeNumberLiteral* WholeNumberLiteral::clone() const
{
    return new WholeNumberLiteral(this->value);
}