#include "finiteListLiteral.h"
#include "stringConverter.h"
#include <vector>

FiniteListLiteral::FiniteListLiteral(const double firstElement, 
                                    const double step, const int numberOfElements)
    : ListLiteral(LiteralType::FINITE_LIST), firstElement(firstElement),
        step(step), numberOfElements(numberOfElements) {}

std::string FiniteListLiteral::toString() const
{
    return StringConverter::finiteListToString(firstElement, step, numberOfElements);
}

FiniteListLiteral* FiniteListLiteral::clone() const
{
    return new FiniteListLiteral(firstElement, step, numberOfElements);
}

Literal* FiniteListLiteral::head() const
{
    return Literal::of(this->firstElement);
}

FiniteListLiteral* FiniteListLiteral::tail() const
{
    return new FiniteListLiteral(this->firstElement + this->step, this->step, this->numberOfElements - 1);
}

int FiniteListLiteral::length() const
{
    return this->numberOfElements;
}