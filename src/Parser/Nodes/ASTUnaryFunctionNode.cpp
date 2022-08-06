#include "ASTUnaryFunctionNode.h"

void ASTUnaryFunctionNode::copy(const ASTUnaryFunctionNode& other)
{
    this->argument = other.argument->clone();
}

void ASTUnaryFunctionNode::deallocate()
{
    delete this->argument;
}

ASTUnaryFunctionNode::ASTUnaryFunctionNode(const Token* token, const ASTNode* argument)
    : ASTNode(token), argument(argument) {}

ASTUnaryFunctionNode::ASTUnaryFunctionNode(const ASTUnaryFunctionNode& other)
    : ASTNode(other)
{
    this->copy(other);
}

ASTUnaryFunctionNode& ASTUnaryFunctionNode::operator = (const ASTUnaryFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocate();
        ASTNode::operator=(other);
        this->copy(other);
    }

    return *this;
}

ASTUnaryFunctionNode::~ASTUnaryFunctionNode()
{
    this->deallocate();
}

std::string ASTUnaryFunctionNode::toString() const
{
    std::string result = "(";
    result += this->token->toString() + " " + this->argument->toString() + ")";

    return result;
}

ASTUnaryFunctionNode* ASTUnaryFunctionNode::clone() const
{
    return new ASTUnaryFunctionNode(*this);
}