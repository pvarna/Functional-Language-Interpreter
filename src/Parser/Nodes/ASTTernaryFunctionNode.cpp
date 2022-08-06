#include "ASTTernaryFunctionNode.h"

void ASTTernaryFunctionNode::copy(const ASTTernaryFunctionNode& other)
{
    this->firstArgument = other.firstArgument->clone();
    this->secondArgument = other.secondArgument->clone();
    this->thirdArgument = other.thirdArgument->clone();
}

void ASTTernaryFunctionNode::deallocate()
{
    delete this->firstArgument;
    delete this->secondArgument;
    delete this->thirdArgument;
}

ASTTernaryFunctionNode::ASTTernaryFunctionNode(const Token* token, const ASTNode* firstArgument,
                        const ASTNode* secondArgument, const ASTNode* thirdArgument)
    : ASTNode(token), firstArgument(firstArgument),
                      secondArgument(secondArgument),
                      thirdArgument(thirdArgument) {}

ASTTernaryFunctionNode::ASTTernaryFunctionNode(const ASTTernaryFunctionNode& other)
    : ASTNode(other)
{
    this->copy(other);
}

ASTTernaryFunctionNode& ASTTernaryFunctionNode::operator = (const ASTTernaryFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocate();
        ASTNode::operator=(other);
        this->copy(other);
    }

    return *this;
}

ASTTernaryFunctionNode::~ASTTernaryFunctionNode()
{
    this->deallocate();
}

std::string ASTTernaryFunctionNode::toString() const
{
    std::string result = "(";
    result += this->token->toString() + " " + this->firstArgument->toString() + " " +
                                              this->secondArgument->toString() + " " +
                                              this->thirdArgument->toString() + ")";

    return result;
}

ASTTernaryFunctionNode* ASTTernaryFunctionNode::clone() const
{
    return new ASTTernaryFunctionNode(*this);
}