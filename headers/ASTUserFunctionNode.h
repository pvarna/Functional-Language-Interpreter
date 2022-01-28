#pragma once

#include "ASTLeafNode.h"
#include <vector>

struct ASTUserFunctionNode : public ASTLeafNode
{
private:
    void copyDefinitionAndArguments(const ASTUserFunctionNode& other);
    void deallocateDefinitionAndArguments();

public:
    const ASTLeafNode* definition;
    std::vector<const ASTLeafNode*> arguments;

    ASTUserFunctionNode(const Token* token, const ASTLeafNode* definition,
                         const std::vector<const ASTLeafNode*>& arguments);
    ASTUserFunctionNode(const ASTUserFunctionNode& other);
    ASTUserFunctionNode& operator = (const ASTUserFunctionNode& other);
    ~ASTUserFunctionNode();

    virtual std::string toString() const override;
};