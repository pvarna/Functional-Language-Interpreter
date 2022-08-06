#pragma once

#include "ASTNode.h"
#include <vector>

struct ASTUserFunctionNode : public ASTNode
{
private:
    void copy(const ASTUserFunctionNode& other);
    void deallocate();

public:
    const ASTNode* definition;

    ASTUserFunctionNode(const Token* token, const ASTNode* definition);
    ASTUserFunctionNode(const ASTUserFunctionNode& other);
    ASTUserFunctionNode& operator = (const ASTUserFunctionNode& other);
    ~ASTUserFunctionNode();

    virtual std::string toString() const override;
    virtual ASTUserFunctionNode* clone() const override;
};