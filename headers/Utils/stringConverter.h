#pragma once

#include <string>
#include <list>

class StringConverter
{
public:
    static std::string intToString(const int value);

    static std::string doubleToString(const double value);

    static std::string listToString(const std::list<double>& values);
};