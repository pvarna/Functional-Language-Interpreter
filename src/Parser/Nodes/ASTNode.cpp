#include "ASTNode.h"
#include "argumentToken.h"
#include "functionNameToken.h"
#include "fractionalNumberToken.h"
#include "wholeNumberToken.h"

void ASTNode::copy(const ASTNode& other)
{
    this->token = other.token->clone();
}

void ASTNode::deallocate()
{
    delete this->token;
}

ASTNode::ASTNode(const Token* token) : token(token->clone()) {}

ASTNode::ASTNode(const ASTNode& other) : token(nullptr)
{
    this->copy(other);
}

ASTNode& ASTNode::operator = (const ASTNode& other)
{
    if (this != &other)
    {
        this->deallocate();
        this->copy(other);
    }

    return *this;
}

ASTNode::~ASTNode()
{
    this->deallocate();
}

std::string ASTNode::toString() const
{
    std::string result = "(";
    result += this->token->toString() + ")";
    
    return result;
}

ASTNode* ASTNode::clone() const
{
    return new ASTNode(*this);
}