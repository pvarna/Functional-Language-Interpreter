#pragma once

#include "ASTLeafNode.h"

struct ASTUnaryFunctionNode : public ASTLeafNode
{
private:
    void copyArgument(const ASTUnaryFunctionNode& other);
    void deallocateArgument();

public:
    const ASTLeafNode* argument;

    ASTUnaryFunctionNode(const Token* token, const ASTLeafNode* argument);
    ASTUnaryFunctionNode(const ASTUnaryFunctionNode& other);
    ASTUnaryFunctionNode& operator = (const ASTUnaryFunctionNode& other);
    ~ASTUnaryFunctionNode();

    virtual std::string toString() const override;
};