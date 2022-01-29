#include "ASTBinaryFunctionNode.h"

void ASTBinaryFunctionNode::copyArguments(const ASTBinaryFunctionNode& other)
{
    this->firstArgument = new ASTNode(*other.firstArgument);
    this->secondArgument = new ASTNode(*other.secondArgument);
}

void ASTBinaryFunctionNode::deallocateArguments()
{
    delete this->firstArgument;
    delete this->secondArgument;
}

ASTBinaryFunctionNode::ASTBinaryFunctionNode(const Token* token, const ASTNode* firstArgument,
                                                                 const ASTNode* secondArgument)
    : ASTNode(token)
{
    this->firstArgument = firstArgument;
    this->secondArgument = secondArgument;
}

ASTBinaryFunctionNode::ASTBinaryFunctionNode(const ASTBinaryFunctionNode& other)
    : ASTNode(token)
{
    this->copyArguments(other);
}

ASTBinaryFunctionNode& ASTBinaryFunctionNode::operator = (const ASTBinaryFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocateArguments();
        ASTNode::operator=(other);
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