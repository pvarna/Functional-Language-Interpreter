#include "wholeNumberLiteral.h"
#include "fractionalNumberLiteral.h"
#include "utils.h"

Literal::Literal(const LiteralType& type) : type(type) {}

Literal* Literal::of(const double number)
{
    if (Utils::isInteger(number))
    {
        return new WholeNumberLiteral(number);
    }

    return new FractionalNumberLiteral(number); 
}