#include "stringConverter.h"
#include "wholeNumberLiteral.h"
#include "fractionalNumberLiteral.h"
#include "userDefinedListLiteral.h"


std::string StringConverter::intToString(const int value)
{
    return std::to_string(value);
}

std::string StringConverter::doubleToString(const double value)
{
    std::string result = std::to_string(value);

    while (result.back() == '0')
    {
        result.pop_back();
    }

    if (result.back() == '.')
    {
        result.pop_back();
    }

    return result;
}

std::string StringConverter::userDefinedListToString(const std::list<const Literal*>& values)
{
    std::string result = "[";

    for (const Literal* current : values)
    {
        result += current->toString() + " ";
    }

    if (result.back() != '[')
    {
        result.pop_back();
    }
    result += "]";

    return result;
}

std::string StringConverter::finiteListToString(const double firstElement, const double step, const int numberOfElements)
{
    std::string result = "[";
    int counter = 0;
    double currentElement = firstElement;

    while (counter < numberOfElements)
    {
        result += doubleToString(currentElement) + " ";

        ++counter;
        currentElement += step;
    }

    if (result.back() != '[')
    {
        result.pop_back();
    }
    result += "]";

    return result;
}

std::string StringConverter::infiniteListToString(const double firstElement, const double step, const int numberOfElementsToBePrinted)
{
    std::string result = "[";
    double currentElement = firstElement;

    for (int i = 0; i < numberOfElementsToBePrinted; ++i)
    {
        result += doubleToString(currentElement) + " ";
        currentElement += step;
    }

    result += "...";

    return result;
}