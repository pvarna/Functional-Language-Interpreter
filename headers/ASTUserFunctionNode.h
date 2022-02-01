#pragma once

#include "ASTNode.h"
#include <vector>

struct ASTUserFunctionNode : public ASTNode
{
private:
    void copyDefinition(const ASTUserFunctionNode& other);
    void deallocateDefinition();

public:
    const ASTNode* definition;

    ASTUserFunctionNode(const Token* token, const ASTNode* definition);
    ASTUserFunctionNode(const ASTUserFunctionNode& other);
    ASTUserFunctionNode& operator = (const ASTUserFunctionNode& other);
    ~ASTUserFunctionNode();

    virtual std::string toString() const override;
};