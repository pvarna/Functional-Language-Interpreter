#include "ASTUserFunctionNode.h"

void ASTUserFunctionNode::copyDefinition(const ASTUserFunctionNode& other)
{
    this->definition = new ASTNode(*other.definition);
}

void ASTUserFunctionNode::deallocateDefinition()
{
    delete this->definition;
}

ASTUserFunctionNode::ASTUserFunctionNode(const Token* token, const ASTNode* definition)
    : ASTNode(token)
{
    this->definition = definition;
}

ASTUserFunctionNode::ASTUserFunctionNode(const ASTUserFunctionNode& other)
    : ASTNode(other)
{
    this->copyDefinition(other);
}

ASTUserFunctionNode& ASTUserFunctionNode::operator = (const ASTUserFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocateDefinition();
        ASTNode::operator=(other);
        this->copyDefinition(other);
    }

    return *this;
}

ASTUserFunctionNode::~ASTUserFunctionNode()
{
    this->deallocateDefinition();
}

std::string ASTUserFunctionNode::toString() const
{
    std::string result = "(";
    result += this->token->toString() + " " + this->definition->toString() + ")";

    return result;
}