#include "ASTNode.h"
#include "argumentToken.h"
#include "functionNameToken.h"
#include "fractionalNumberToken.h"
#include "wholeNumberToken.h"

void ASTNode::copyToken(const Token* token)
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

void ASTNode::deallocateToken()
{
    delete this->token;
}

ASTNode::ASTNode(const Token* token)
{
    this->token = token;
}

ASTNode::ASTNode(const ASTNode& other)
{
    this->copyToken(other.token);
}

ASTNode& ASTNode::operator = (const ASTNode& other)
{
    if (this != &other)
    {
        this->deallocateToken();
        this->copyToken(other.token);
    }

    return *this;
}

ASTNode::~ASTNode()
{
    this->deallocateToken();
}

std::string ASTNode::toString() const
{
    std::string result = "(";
    result += this->token->toString() + ")";
    
    return result;
}