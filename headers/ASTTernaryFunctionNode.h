#pragma once

#include "ASTLeafNode.h"

struct ASTTernaryFunctionNode : public ASTLeafNode
{
private:
    void copyArguments(const ASTTernaryFunctionNode& other);
    void deallocateArguments();

public:
    const ASTLeafNode* firstArgument;
    const ASTLeafNode* secondArgument;
    const ASTLeafNode* thirdArgument;

    ASTTernaryFunctionNode(const Token* token, const ASTLeafNode* firstArgument,
                        const ASTLeafNode* secondArgument, const ASTLeafNode* thirdArgument);
    ASTTernaryFunctionNode(const ASTTernaryFunctionNode& other);
    ASTTernaryFunctionNode& operator = (const ASTTernaryFunctionNode& other);
    ~ASTTernaryFunctionNode();

    virtual std::string toString() const override;
};