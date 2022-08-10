#pragma once

#include "listLiteral.h"

struct FiniteListLiteral : public ListLiteral
{
    double firstElement;
    double step;
    int numberOfElements;

    FiniteListLiteral(const double firstElement, const double step, const int numberOfElements);

    virtual std::string toString() const override;
    virtual FiniteListLiteral* clone() const override;

    virtual Literal* head() const override;
    virtual FiniteListLiteral* tail() const override; 
    virtual int length() const override;
    virtual bool isInfinite() const override;
    virtual bool toBool() const override;
};