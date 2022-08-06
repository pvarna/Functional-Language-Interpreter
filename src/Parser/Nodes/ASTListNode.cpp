#include "ASTListNode.h"

void ASTListNode::copy(const ASTListNode& other)
{
    for (const ASTNode* current : other.elements)
    {
        this->elements.push_back(current->clone());
    }
}

void ASTListNode::deallocate()
{
    for (const ASTNode* current : this->elements)
    {
        delete current;
    }
}

ASTListNode::ASTListNode(const Token* token, const std::vector<const ASTNode*>& elements)
    : ASTNode(token)
{
    for (const ASTNode* current : elements)
    {
        this->elements.push_back(current);
    }
}

ASTListNode::ASTListNode(const ASTListNode& other)
    : ASTNode(other)
{
    this->copy(other);
}

ASTListNode& ASTListNode::operator = (const ASTListNode& other)
{
    if (this != &other)
    {
        this->deallocate();
        ASTNode::operator=(other);
        this->copy(other);
    }

    return *this;
}

ASTListNode::~ASTListNode()
{
    this->deallocate();
}

std::string ASTListNode::toString() const
{
    std::string result = "[";

    for (const ASTNode* current : this->elements)
    {
        result += " " + current->toString();
    }
    result += " ]";

    return result;
}

ASTListNode* ASTListNode::clone() const
{
    return new ASTListNode(*this);
}