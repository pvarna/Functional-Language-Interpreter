#include "ASTLeafNode.h"
#include "argumentToken.h"
#include "functionNameToken.h"
#include "fractionalNumberToken.h"
#include "wholeNumberToken.h"

void ASTLeafNode::copyToken(const Token* token)
{
    if (!token)
    {
        this->token = nullptr;
    }
    if (token->type == TokenType::ARGUMENT)
    {
        const ArgumentToken* argumentToken = dynamic_cast<const ArgumentToken*>(token);

        this->token = new ArgumentToken(argumentToken->index);
    }
    else if (token->type == TokenType::FUNCTION_NAME)
    {
        const FunctionNameToken* functionNameToken = dynamic_cast<const FunctionNameToken*>(token);

        this->token = new FunctionNameToken(functionNameToken->name);
    }
    else if (token->type == TokenType::FRACTIONAL_NUMBER)
    {
        const FractionalNumberToken* fractionalNumberToken = dynamic_cast<const FractionalNumberToken*>(token);

        this->token = new FractionalNumberToken(fractionalNumberToken->value);
    }
    else if (token->type == TokenType::WHOLE_NUMBER)
    {
        const WholeNumberToken* wholeNumberToken = dynamic_cast<const WholeNumberToken*>(token);

        this->token = new WholeNumberToken(wholeNumberToken->value);
    }
    else
    {
        this->token = new Token(token->type);
    }
}

void ASTLeafNode::deallocateToken()
{
    delete this->token;
}

ASTLeafNode::ASTLeafNode(const Token* token)
{
    this->token = token;
}

ASTLeafNode::ASTLeafNode(const ASTLeafNode& other)
{
    this->copyToken(other.token);
}

ASTLeafNode& ASTLeafNode::operator = (const ASTLeafNode& other)
{
    if (this != &other)
    {
        this->deallocateToken();
        this->copyToken(other.token);
    }

    return *this;
}

ASTLeafNode::~ASTLeafNode()
{
    this->deallocateToken();
}

std::string ASTLeafNode::toString() const
{
    return this->token->toString();
}