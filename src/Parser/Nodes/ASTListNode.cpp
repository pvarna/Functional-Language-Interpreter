#include "ASTListNode.h"

void ASTListNode::copyElements(const ASTListNode& other)
{
    for (const ASTNode* current : other.elements)
    {
        this->elements.push_back(new ASTNode(*current));
    }
}

void ASTListNode::deallocateElements()
{
    for (const ASTNode* current : this->elements)
    {
        delete current;
    }
}

ASTListNode::ASTListNode(const Token* token, const std::vector<const ASTNode*>& elements)
    : ASTNode(token)
{
    this->elements = elements;
}

ASTListNode::ASTListNode(const ASTListNode& other)
    : ASTNode(token)
{
    this->copyElements(other);
}

ASTListNode& ASTListNode::operator = (const ASTListNode& other)
{
    if (this != &other)
    {
        this->deallocateElements();
        ASTNode::operator=(other);
        this->copyElements(other);
    }

    return *this;
}

ASTListNode::~ASTListNode()
{
    this->deallocateElements();
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