#include "utils.h"
#include <cmath>

bool Utils::isInteger(const double number)
{
    return std::floor(number) == std::ceil(number);
}