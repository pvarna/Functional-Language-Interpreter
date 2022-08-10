#pragma once

#include "listLiteral.h"

class ConcatenatedListLiteral : public ListLiteral
{
private:
    ListLiteral* first;
    ListLiteral* second;

    void copy(const ConcatenatedListLiteral& other);
    void deallocate();

public:
    ConcatenatedListLiteral(const ListLiteral* first, const ListLiteral* second);
    ConcatenatedListLiteral(const ConcatenatedListLiteral& other);
    ConcatenatedListLiteral& operator = (const ConcatenatedListLiteral& other);
    ~ConcatenatedListLiteral();

    virtual std::string toString() const override;
    virtual ConcatenatedListLiteral* clone() const override;

    virtual Literal* head() const override;
    virtual ListLiteral* tail() const override;
    virtual int length() const override;
    virtual bool isInfinite() const override;
    virtual bool toBool() const;
};