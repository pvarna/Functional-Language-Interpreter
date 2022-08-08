#include "userDefinedListLiteral.h"
#include "stringConverter.h"

void UserDefinedListLiteral::copy(const UserDefinedListLiteral& other)
{
    for (const Literal* current : other.elements)
    {
        this->elements.push_back(current->clone());
    }
}

void UserDefinedListLiteral::deallocate()
{
    for (const Literal* current : this->elements)
    {
        delete current;
    }

    this->elements.clear();
}

UserDefinedListLiteral::UserDefinedListLiteral(const std::list<const Literal*>& elements)
    : Literal(LiteralType::USER_DEFINED_LIST)
{
    for (const Literal* current : elements)
    {
        this->elements.push_back(current->clone());
    }
}

UserDefinedListLiteral::UserDefinedListLiteral(const UserDefinedListLiteral& other)
    : Literal(other)
{
    this->copy(other);
}

UserDefinedListLiteral& UserDefinedListLiteral::operator = (const UserDefinedListLiteral& other)
{
    if (this != &other)
    {
        this->deallocate();
        Literal::operator=(other);
        this->copy(other);
    }

    return *this;
}

UserDefinedListLiteral::~UserDefinedListLiteral()
{
    this->deallocate();
}

std::string UserDefinedListLiteral::toString() const
{
    return StringConverter::userDefinedListToString(this->elements);
}

UserDefinedListLiteral* UserDefinedListLiteral::clone() const
{
    return new UserDefinedListLiteral(this->elements);
}