#include "functionDefinedListLiteral.h"
#include <vector>

FunctionDefinedListLiteral::FunctionDefinedListLiteral(const double firstElement, 
                                    const double step, const int numberOfElements)
    : Literal(LiteralType::FUNCTION_DEFINED_LIST)
{
    this->isInfinite = (numberOfElements < 0);
    this->firstElement = firstElement;
    this->step = step;
    this->numberOfElements = numberOfElements;
}

std::string FunctionDefinedListLiteral::toString() const
{
    if (!this->isInfinite)
    {
        std::list<double> values;
        double current = this->firstElement;
        for (int i = 0; i < this->numberOfElements; ++i)
        {
            values.push_back(current);

            current += this->step;
        }

        return StringConverter::listToString(values);
    }

    return "";
}