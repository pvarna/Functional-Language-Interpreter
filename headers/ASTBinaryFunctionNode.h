#pragma once

#include "ASTLeafNode.h"

struct ASTBinaryFunctionNode : public ASTLeafNode
{
private:
    void copyArguments(const ASTBinaryFunctionNode& other);
    void deallocateArguments();

public:
    const ASTLeafNode* firstArgument;
    const ASTLeafNode* secondArgument;

    ASTBinaryFunctionNode(const Token* token, const ASTLeafNode* firstArgument,
                                              const ASTLeafNode* secondArgument);
    ASTBinaryFunctionNode(const ASTBinaryFunctionNode& other);
    ASTBinaryFunctionNode& operator = (const ASTBinaryFunctionNode& other);
    ~ASTBinaryFunctionNode();

    virtual std::string toString() const override;
};