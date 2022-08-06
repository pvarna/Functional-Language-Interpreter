#include "ASTNode.h"
#include "argumentToken.h"
#include "functionNameToken.h"
#include "fractionalNumberToken.h"
#include "wholeNumberToken.h"

void ASTNode::copy(const ASTNode& other)
{
    if (!other.token)
    {
        return;
    }

    this->token = other.token->clone();
}

void ASTNode::deallocate()
{
    delete this->token;
}

ASTNode::ASTNode(const Token* token) : token(nullptr)
{
    if (!token)
    {
        throw std::invalid_argument("The token shouldn't be nullptr");
    }
    this->token = token->clone();
}

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