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
    : ListLiteral(LiteralType::USER_DEFINED_LIST)
{
    for (const Literal* current : elements)
    {
        this->elements.push_back(current->clone());
    }
}

UserDefinedListLiteral::UserDefinedListLiteral(const UserDefinedListLiteral& other)
    : ListLiteral(other)
{
    this->copy(other);
}

UserDefinedListLiteral& UserDefinedListLiteral::operator = (const UserDefinedListLiteral& other)
{
    if (this != &other)
    {
        this->deallocate();
        ListLiteral::operator=(other);
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
    return new UserDefinedListLiteral(*this);
}

Literal* UserDefinedListLiteral::head() const
{
    if (this->elements.empty())
    {
        return nullptr;
    }

    return this->elements.front()->clone();
}

UserDefinedListLiteral* UserDefinedListLiteral::tail() const
{
    if (this->elements.empty())
    {
        return nullptr;
    }

    std::list<const Literal*>::const_iterator begin = this->elements.begin();
    ++begin;

    std::list<const Literal*> elements;
    for (std::list<const Literal*>::const_iterator it = begin; it != this->elements.end(); ++it)
    {
        elements.push_back(*it);
    }

    return new UserDefinedListLiteral(elements);
}

int UserDefinedListLiteral::length() const
{
    return this->elements.size();
}

bool UserDefinedListLiteral::isInfinite() const
{
    return false;
}