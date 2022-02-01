#include "ASTTernaryFunctionNode.h"

void ASTTernaryFunctionNode::copyArguments(const ASTTernaryFunctionNode& other)
{
    this->firstArgument = new ASTNode(*other.firstArgument);
    this->secondArgument = new ASTNode(*other.secondArgument);
    this->thirdArgument = new ASTNode(*other.thirdArgument);
}

void ASTTernaryFunctionNode::deallocateArguments()
{
    delete this->firstArgument;
    delete this->secondArgument;
    delete this->thirdArgument;
}

ASTTernaryFunctionNode::ASTTernaryFunctionNode(const Token* token, const ASTNode* firstArgument,
                        const ASTNode* secondArgument, const ASTNode* thirdArgument)
    : ASTNode(token)
{
    this->firstArgument = firstArgument;
    this->secondArgument = secondArgument;
    this->thirdArgument = thirdArgument;
}

ASTTernaryFunctionNode::ASTTernaryFunctionNode(const ASTTernaryFunctionNode& other)
    : ASTNode(other)
{
    this->copyArguments(other);
}

ASTTernaryFunctionNode& ASTTernaryFunctionNode::operator = (const ASTTernaryFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocateArguments();
        ASTNode::operator=(other);
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