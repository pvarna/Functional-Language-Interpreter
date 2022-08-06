#include "ASTN_aryFunctionNode.h"

void ASTN_aryFunctionNode::copy(const ASTN_aryFunctionNode& other)
{
    for (const ASTNode* current : other.arguments)
    {
        this->arguments.push_back(current->clone());
    }
}

void ASTN_aryFunctionNode::deallocate()
{
    for (const ASTNode* current : this->arguments)
    {
        delete current;
    }
    this->arguments.clear();
}

ASTN_aryFunctionNode::ASTN_aryFunctionNode(const Token* token, const std::vector<const ASTNode*>& arguments)
    : ASTNode(token)
{
    for (const ASTNode* current : arguments)
    {
        this->arguments.push_back(current);
    }
}

ASTN_aryFunctionNode::ASTN_aryFunctionNode(const ASTN_aryFunctionNode& other)
    : ASTNode(other)
{
    this->copy(other);
}

ASTN_aryFunctionNode& ASTN_aryFunctionNode::operator = (const ASTN_aryFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocate();
        ASTNode::operator=(other);
        this->copy(other);
    }

    return *this;
}

ASTN_aryFunctionNode::~ASTN_aryFunctionNode()
{
    this->deallocate();
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

ASTN_aryFunctionNode* ASTN_aryFunctionNode::clone() const
{
    return new ASTN_aryFunctionNode(*this);
}