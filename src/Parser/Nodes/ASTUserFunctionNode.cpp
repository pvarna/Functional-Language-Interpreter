#include "ASTUserFunctionNode.h"

void ASTUserFunctionNode::copyDefinitionAndArguments(const ASTUserFunctionNode& other)
{
    this->definition = new ASTNode(*other.definition);
    for (const ASTNode* current : other.arguments)
    {
        this->arguments.push_back(new ASTNode(*current));
    }
}

void ASTUserFunctionNode::deallocateDefinitionAndArguments()
{
    delete this->definition;
    for (const ASTNode* current : this->arguments)
    {
        delete current;
    }
}

ASTUserFunctionNode::ASTUserFunctionNode(const Token* token, const ASTNode* definition,
                         const std::vector<const ASTNode*>& arguments)
    : ASTNode(token)
{
    this->definition = definition;
    this->arguments = arguments;
}

ASTUserFunctionNode::ASTUserFunctionNode(const ASTUserFunctionNode& other)
    : ASTNode(token)
{
    this->copyDefinitionAndArguments(other);
}

ASTUserFunctionNode& ASTUserFunctionNode::operator = (const ASTUserFunctionNode& other)
{
    if (this != &other)
    {
        this->deallocateDefinitionAndArguments();
        ASTNode::operator=(other);
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
    for (const ASTNode* current : this->arguments)
    {
        result += " " + current->toString();
    }
    result += ")";

    return result;
}