#include "ASTN_aryFunctionNode.h"

void ASTN_aryFunctionNode::copyArguments(const ASTN_aryFunctionNode& other)
{
    for (const ASTNode* current : other.arguments)
    {
        this->arguments.push_back(new ASTNode(*current));
    }
}

void ASTN_aryFunctionNode::deallocateArguments()
{
    for (const ASTNode* current : this->arguments)
    {
        delete current;
    }
}

ASTN_aryFunctionNode::ASTN_aryFunctionNode(const Token* token, const std::vector<const ASTNode*>& arguments)
    : ASTNode(token)
{
    this->arguments = arguments;
}

ASTN_aryFunctionNode::ASTN_aryFunctionNode(const ASTN_aryFunctionNode& other)
    : ASTNode(token)
{
    this->copyArguments(other);
}

ASTN_aryFunctionNode& ASTN_aryFunctionNode::operator = (const ASTN_aryFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocateArguments();
        ASTNode::operator=(other);
        this->copyArguments(other);
    }

    return *this;
}

ASTN_aryFunctionNode::~ASTN_aryFunctionNode()
{
    this->deallocateArguments();
}

std::string ASTN_aryFunctionNode::toString() const
{
    std::string result = "(";

    result += this->token->toString();

    for (const ASTNode* current : this->arguments)
    {
        result += " " + current->toString();
    }
    result += ")";

    return result;
}