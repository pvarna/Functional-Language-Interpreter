#include "concatenatedListLiteral.h"

void ConcatenatedListLiteral::copy(const ConcatenatedListLiteral& other)
{
    this->first = other.first->clone();
    this->second = other.second->clone();
}

void ConcatenatedListLiteral::deallocate()
{
    delete this->first;
    delete this->second;
}

ConcatenatedListLiteral::ConcatenatedListLiteral(const ListLiteral* first, const ListLiteral* second)
    : ListLiteral(LiteralType::CONCATENATED_LIST),
     first(first->clone()), second(second->clone()) {}

ConcatenatedListLiteral::ConcatenatedListLiteral(const ConcatenatedListLiteral& other)
    : ListLiteral(other)
{
    this->copy(other);
}

ConcatenatedListLiteral& ConcatenatedListLiteral::operator = (const ConcatenatedListLiteral& other)
{
    if (this != &other)
    {
        this->deallocate();
        ListLiteral::operator=(other);
        this->copy(other);
    }

    return *this;
}

ConcatenatedListLiteral::~ConcatenatedListLiteral()
{
    this->deallocate();
}

std::string ConcatenatedListLiteral::toString() const
{
    std::string result = first->toString();
    result.pop_back();
    result += " ";
    result += second->toString().substr(1);

    return result;
}

ConcatenatedListLiteral* ConcatenatedListLiteral::clone() const
{
    return new ConcatenatedListLiteral(*this);
}

Literal* ConcatenatedListLiteral::head() const
{
    if (this->first->length() > 0)
    {
        return this->first->head();
    }

    return this->second->head();
}

ListLiteral* ConcatenatedListLiteral::tail() const
{
    if (this->first->length() > 0)
    {
        return this->first->tail();
    }

    ListLiteral* firstTail = this->first->tail();
    ListLiteral* result = new ConcatenatedListLiteral(firstTail, second);

    delete firstTail;
    return result;
}

int ConcatenatedListLiteral::length() const
{
    int length1 = this->first->length();
    int length2 = this->second->length();

    return this->first->length() + this->second->length();
}

bool ConcatenatedListLiteral::isInfinite() const
{
    return this->first->isInfinite() || this->second->isInfinite();
}