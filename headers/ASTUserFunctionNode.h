#pragma once

#include "ASTNode.h"
#include <vector>

struct ASTUserFunctionNode : public ASTNode
{
private:
    void copyDefinitionAndArguments(const ASTUserFunctionNode& other);
    void deallocateDefinitionAndArguments();

public:
    const ASTNode* definition;
    std::vector<const ASTNode*> arguments;

    ASTUserFunctionNode(const Token* token, const ASTNode* definition,
                         const std::vector<const ASTNode*>& arguments);
    ASTUserFunctionNode(const ASTUserFunctionNode& other);
    ASTUserFunctionNode& operator = (const ASTUserFunctionNode& other);
    ~ASTUserFunctionNode();

    virtual std::string toString() const override;
};