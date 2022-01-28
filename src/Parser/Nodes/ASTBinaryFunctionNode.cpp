#include "ASTBinaryFunctionNode.h"

void ASTBinaryFunctionNode::copyArguments(const ASTBinaryFunctionNode& other)
{
    this->firstArgument = new ASTLeafNode(*other.firstArgument);
    this->secondArgument = new ASTLeafNode(*other.secondArgument);
}

void ASTBinaryFunctionNode::deallocateArguments()
{
    delete this->firstArgument;
    delete this->secondArgument;
}

ASTBinaryFunctionNode::ASTBinaryFunctionNode(const Token* token, const ASTLeafNode* firstArgument,
                                                                 const ASTLeafNode* secondArgument)
    : ASTLeafNode(token)
{
    this->firstArgument = firstArgument;
    this->secondArgument = secondArgument;
}

ASTBinaryFunctionNode::ASTBinaryFunctionNode(const ASTBinaryFunctionNode& other)
    : ASTLeafNode(token)
{
    this->copyArguments(other);
}

ASTBinaryFunctionNode& ASTBinaryFunctionNode::operator = (const ASTBinaryFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocateArguments();
        ASTLeafNode::operator=(other);
        this->copyArguments(other);
    }

    return *this;
}

ASTBinaryFunctionNode::~ASTBinaryFunctionNode()
{
    this->deallocateArguments();
}

std::string ASTBinaryFunctionNode::toString() const
{
    std::string result = "(";
    result += this->token->toString() + " " + this->firstArgument->toString() + " " +
                                              this->secondArgument->toString() + ")";

    return result;
}