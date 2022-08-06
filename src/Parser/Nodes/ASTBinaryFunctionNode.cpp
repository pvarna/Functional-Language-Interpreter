#include "ASTBinaryFunctionNode.h"

void ASTBinaryFunctionNode::copy(const ASTBinaryFunctionNode& other)
{
    this->firstArgument = other.firstArgument->clone();
    this->secondArgument = other.secondArgument->clone();
}

void ASTBinaryFunctionNode::deallocate()
{
    delete this->firstArgument;
    delete this->secondArgument;
}

ASTBinaryFunctionNode::ASTBinaryFunctionNode(const Token* token, const ASTNode* firstArgument,
                                                                 const ASTNode* secondArgument)
    : ASTNode(token), firstArgument(firstArgument),
                      secondArgument(secondArgument) {}

ASTBinaryFunctionNode::ASTBinaryFunctionNode(const ASTBinaryFunctionNode& other)
    : ASTNode(other)
{
    this->copy(other);
}

ASTBinaryFunctionNode& ASTBinaryFunctionNode::operator = (const ASTBinaryFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocate();
        ASTNode::operator=(other);
        this->copy(other);
    }

    return *this;
}

ASTBinaryFunctionNode::~ASTBinaryFunctionNode()
{
    this->deallocate();
}

std::string ASTBinaryFunctionNode::toString() const
{
    std::string result = "(";
    result += this->token->toString() + " " + this->firstArgument->toString() + " " +
                                              this->secondArgument->toString() + ")";

    return result;
}

ASTBinaryFunctionNode* ASTBinaryFunctionNode::clone() const
{
    return new ASTBinaryFunctionNode(*this);   
}