#pragma once

#include "literal.h"
#include <string>
#include <list>

class StringConverter
{
public:
    static std::string intToString(const int value);

    static std::string doubleToString(const double value);

    static std::string userDefinedListToString(const std::list<const Literal*>& elements);
    static std::string finiteListToString(const double firstElement, const double step, const int numberOfElements);
    static std::string infiniteListToString(const double firstElement, const double step, const int numberOfElementsToBePrinted);
};