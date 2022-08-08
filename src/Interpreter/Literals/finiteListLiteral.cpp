#include "finiteListLiteral.h"
#include "stringConverter.h"
#include <vector>

FiniteListLiteral::FiniteListLiteral(const double firstElement, 
                                    const double step, const int numberOfElements)
    : Literal(LiteralType::FINITE_LIST), firstElement(firstElement),
        step(step), numberOfElements(numberOfElements) {}

std::string FiniteListLiteral::toString() const
{
    return StringConverter::finiteListToString(firstElement, step, numberOfElements);
}

FiniteListLiteral* FiniteListLiteral::clone() const
{
    return new FiniteListLiteral(firstElement, step, numberOfElements);
}