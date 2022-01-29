#include "ASTUnaryFunctionNode.h"

void ASTUnaryFunctionNode::copyArgument(const ASTUnaryFunctionNode& other)
{
    this->argument = new ASTNode(*other.argument);
}

void ASTUnaryFunctionNode::deallocateArgument()
{
    delete this->argument;
}

ASTUnaryFunctionNode::ASTUnaryFunctionNode(const Token* token, const ASTNode* argument)
    : ASTNode(token)
{
    this->argument = argument;
}

ASTUnaryFunctionNode::ASTUnaryFunctionNode(const ASTUnaryFunctionNode& other)
    : ASTNode(token)
{
    this->copyArgument(other);
}

ASTUnaryFunctionNode& ASTUnaryFunctionNode::operator = (const ASTUnaryFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocateArgument();
        ASTNode::operator=(other);
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