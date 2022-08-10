#pragma once

#include "listLiteral.h"

struct InfiniteListLiteral : public ListLiteral
{
    static const int COUNT_OF_ELEMENTS_TO_BE_PRINTED = 8;

    double firstElement;
    double step;

    InfiniteListLiteral(const double firstElement, const double step = 1);

    virtual std::string toString() const override;
    virtual InfiniteListLiteral* clone() const override;

    virtual Literal* head() const override;
    virtual InfiniteListLiteral* tail() const override;
    virtual int length() const override;
    virtual bool isInfinite() const override;
    virtual bool toBool() const override;
};