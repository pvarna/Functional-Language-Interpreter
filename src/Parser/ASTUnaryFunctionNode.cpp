#include "ASTUnaryFunctionNode.h"

void ASTUnaryFunctionNode::copyArgument(const ASTUnaryFunctionNode& other)
{
    this->argument = new ASTLeafNode(*other.argument);
}

void ASTUnaryFunctionNode::deallocateArgument()
{
    delete this->argument;
}

ASTUnaryFunctionNode::ASTUnaryFunctionNode(const Token* token, const ASTLeafNode* argument)
    : ASTLeafNode(token)
{
    this->argument = argument;
}

ASTUnaryFunctionNode::ASTUnaryFunctionNode(const ASTUnaryFunctionNode& other)
    : ASTLeafNode(token)
{
    this->copyArgument(other);
}

ASTUnaryFunctionNode& ASTUnaryFunctionNode::operator = (const ASTUnaryFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocateArgument();
        ASTLeafNode::operator=(other);
        this->copyArgument(other);
    }

    return *this;
}

ASTUnaryFunctionNode::~ASTUnaryFunctionNode()
{
    this->deallocateArgument();
}

std::string ASTUnaryFunctionNode::toString() const
{
    std::string result = "(";
    result += this->token->toString() + " " + this->argument->toString() + ")";

    return result;
}