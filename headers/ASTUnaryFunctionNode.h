#pragma once

#include "ASTNode.h"

struct ASTUnaryFunctionNode : public ASTNode
{
private:
    void copyArgument(const ASTUnaryFunctionNode& other);
    void deallocateArgument();

public:
    const ASTNode* argument;

    ASTUnaryFunctionNode(const Token* token, const ASTNode* argument);
    ASTUnaryFunctionNode(const ASTUnaryFunctionNode& other);
    ASTUnaryFunctionNode& operator = (const ASTUnaryFunctionNode& other);
    ~ASTUnaryFunctionNode();

    virtual std::string toString() const override;
};