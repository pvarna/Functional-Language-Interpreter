#pragma once

#include "listLiteral.h"
#include <list>

struct UserDefinedListLiteral : public ListLiteral
{
private:
    void copy(const UserDefinedListLiteral& other);
    void deallocate();

public:
    std::list<const Literal*> elements;

    UserDefinedListLiteral(const std::list<const Literal*>& elements);
    UserDefinedListLiteral(const UserDefinedListLiteral& other);
    UserDefinedListLiteral& operator = (const UserDefinedListLiteral& other);
    virtual ~UserDefinedListLiteral();

    virtual std::string toString() const override;
    virtual UserDefinedListLiteral* clone() const override;

    virtual Literal* head() const override;
    virtual UserDefinedListLiteral* tail() const override;
    virtual int length() const override;
};