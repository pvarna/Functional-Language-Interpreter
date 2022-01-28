#include "ASTUserFunctionNode.h"

void ASTUserFunctionNode::copyDefinitionAndArguments(const ASTUserFunctionNode& other)
{
    this->definition = new ASTLeafNode(*other.definition);
    for (const ASTLeafNode* current : other.arguments)
    {
        this->arguments.push_back(new ASTLeafNode(*current));
    }
}

void ASTUserFunctionNode::deallocateDefinitionAndArguments()
{
    delete this->definition;
    for (const ASTLeafNode* current : this->arguments)
    {
        delete current;
    }
}

ASTUserFunctionNode::ASTUserFunctionNode(const Token* token, const ASTLeafNode* definition,
                         const std::vector<const ASTLeafNode*>& arguments)
    : ASTLeafNode(token)
{
    this->definition = definition;
    this->arguments = arguments;
}

ASTUserFunctionNode::ASTUserFunctionNode(const ASTUserFunctionNode& other)
    : ASTLeafNode(token)
{
    this->copyDefinitionAndArguments(other);
}

ASTUserFunctionNode& ASTUserFunctionNode::operator = (const ASTUserFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocateDefinitionAndArguments();
        ASTLeafNode::operator=(other);
        this->copyDefinitionAndArguments(other);
    }

    return *this;
}

ASTUserFunctionNode::~ASTUserFunctionNode()
{
    this->deallocateDefinitionAndArguments();
}

std::string ASTUserFunctionNode::toString() const
{
    std::string result = "(";
    result += this->token->toString() + " " + this->definition->toString();
    for (const ASTLeafNode* current : this->arguments)
    {
        result += " " + current->toString();
    }
    result += ")";

    return result;
}