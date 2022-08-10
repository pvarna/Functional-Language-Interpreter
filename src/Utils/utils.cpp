#include "utils.h"
#include <cmath>

const double Utils::EPS = 0.00001;

bool Utils::isInteger(const double number)
{
    return std::floor(number) == std::ceil(number);
}