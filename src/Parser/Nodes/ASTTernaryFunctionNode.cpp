#include "ASTTernaryFunctionNode.h"

void ASTTernaryFunctionNode::copyArguments(const ASTTernaryFunctionNode& other)
{
    this->firstArgument = new ASTLeafNode(*other.firstArgument);
    this->secondArgument = new ASTLeafNode(*other.secondArgument);
    this->thirdArgument = new ASTLeafNode(*other.thirdArgument);
}

void ASTTernaryFunctionNode::deallocateArguments()
{
    delete this->firstArgument;
    delete this->secondArgument;
    delete this->thirdArgument;
}

ASTTernaryFunctionNode::ASTTernaryFunctionNode(const Token* token, const ASTLeafNode* firstArgument,
                        const ASTLeafNode* secondArgument, const ASTLeafNode* thirdArgument)
    : ASTLeafNode(token)
{
    this->firstArgument = firstArgument;
    this->secondArgument = secondArgument;
    this->thirdArgument = thirdArgument;
}

ASTTernaryFunctionNode::ASTTernaryFunctionNode(const ASTTernaryFunctionNode& other)
    : ASTLeafNode(token)
{
    this->copyArguments(other);
}

ASTTernaryFunctionNode& ASTTernaryFunctionNode::operator = (const ASTTernaryFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocateArguments();
        ASTLeafNode::operator=(other);
        this->copyArguments(other);
    }

    return *this;
}

ASTTernaryFunctionNode::~ASTTernaryFunctionNode()
{
    this->deallocateArguments();
}

std::string ASTTernaryFunctionNode::toString() const
{
    std::string result = "(";
    result += this->token->toString() + " " + this->firstArgument->toString() + " " +
                                              this->secondArgument->toString() + " " +
                                              this->thirdArgument->toString() + ")";

    return result;
}