#include "ASTUserFunctionNode.h"

void ASTUserFunctionNode::copy(const ASTUserFunctionNode& other)
{
    this->definition = other.definition->clone();
}

void ASTUserFunctionNode::deallocate()
{
    delete this->definition;
}

ASTUserFunctionNode::ASTUserFunctionNode(const Token* token, const ASTNode* definition)
    : ASTNode(token), definition(definition) {}

ASTUserFunctionNode::ASTUserFunctionNode(const ASTUserFunctionNode& other)
    : ASTNode(other)
{
    this->copy(other);
}

ASTUserFunctionNode& ASTUserFunctionNode::operator = (const ASTUserFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocate();
        ASTNode::operator=(other);
        this->copy(other);
    }

    return *this;
}

ASTUserFunctionNode::~ASTUserFunctionNode()
{
    this->deallocate();
}

std::string ASTUserFunctionNode::toString() const
{
    std::string result = "(";
    result += this->token->toString() + " " + this->definition->toString() + ")";

    return result;
}

ASTUserFunctionNode* ASTUserFunctionNode::clone() const
{
    return new ASTUserFunctionNode(*this);
}