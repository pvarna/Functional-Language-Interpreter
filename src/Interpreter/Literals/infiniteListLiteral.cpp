#include "infiniteListLiteral.h"
#include "stringConverter.h"
#include <stdexcept>

InfiniteListLiteral::InfiniteListLiteral(const double firstElement, const double step)
    : ListLiteral(LiteralType::INFINITE_LIST), 
        firstElement(firstElement), step(step) {}

std::string InfiniteListLiteral::toString() const
{
    return StringConverter::infiniteListToString(firstElement, step, COUNT_OF_ELEMENTS_TO_BE_PRINTED);
}

InfiniteListLiteral* InfiniteListLiteral::clone() const
{
    return new InfiniteListLiteral(this->firstElement, this->step);
}

Literal* InfiniteListLiteral::head() const
{
    return Literal::of(this->firstElement);
}

InfiniteListLiteral* InfiniteListLiteral::tail() const
{
    return new InfiniteListLiteral(this->firstElement + this->step, this->step);
}

int InfiniteListLiteral::length() const
{
    throw std::runtime_error("The length of an infinite list is undefined");
}

bool InfiniteListLiteral::isInfinite() const
{
    return true;
}

bool InfiniteListLiteral::toBool() const
{
    return true;
}