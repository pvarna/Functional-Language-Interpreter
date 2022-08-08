#include "infiniteListLiteral.h"
#include "stringConverter.h"

InfiniteListLiteral::InfiniteListLiteral(const double firstElement, const double step)
    : Literal(LiteralType::INFINITE_LIST), 
        firstElement(firstElement), step(step) {}

std::string InfiniteListLiteral::toString() const
{
    return StringConverter::infiniteListToString(firstElement, step, COUNT_OF_ELEMENTS_TO_BE_PRINTED);
}