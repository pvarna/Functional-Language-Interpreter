#include "wholeNumberLiteral.h"
#include "fractionalNumberLiteral.h"
#include "utils.h"
#include <stdexcept>

Literal::Literal(const LiteralType& type) : type(type) {}

Literal* Literal::of(const double number)
{
    if (Utils::isInteger(number))
    {
        return new WholeNumberLiteral(number);
    }

    return new FractionalNumberLiteral(number); 
}

bool Literal::eq(const Literal* first, const Literal* second)
{
    int firstLength = first->length();
    int secondLength = second->length();

    if (firstLength == secondLength)
    {
        return first->toString() == second->toString();
    }

    if (firstLength == -1 && secondLength == 1)
    {
        std::string secondStr = second->toString();
        return first->toString() == secondStr.substr(1, secondStr.size() - 2);
    }

    if (secondLength == -1 && firstLength == 1)
    {
        std::string firstStr = first->toString();
        return second->toString() == firstStr.substr(1, firstStr.size() - 2);
    }

    return false;
}

double Literal::valueOf(const Literal* literal)
{
    switch (literal->type)
    {
    case LiteralType::WHOLE_NUMBER:
        return dynamic_cast<const WholeNumberLiteral*>(literal)->value;
    case LiteralType::FRACTIONAL_NUMBER:
        return dynamic_cast<const FractionalNumberLiteral*>(literal)->value;
    default:
        throw std::runtime_error("Expected a number");
    }
}