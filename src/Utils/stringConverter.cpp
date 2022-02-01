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

std::string StringConverter::listToString(const std::list<double>& values)
{
    std::string result = "[";

    for (double current : values)
    {
        result += StringConverter::doubleToString(current) + " ";
    }

    if (result.back() != '[')
    {
        result.pop_back();
    }
    result += "]";

    return result;
}