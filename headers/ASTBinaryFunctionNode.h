#pragma once

#include "ASTNode.h"

struct ASTBinaryFunctionNode : public ASTNode
{
private:
    void copyArguments(const ASTBinaryFunctionNode& other);
    void deallocateArguments();

public:
    const ASTNode* firstArgument;
    const ASTNode* secondArgument;

    ASTBinaryFunctionNode(const Token* token, const ASTNode* firstArgument,
                                              const ASTNode* secondArgument);
    ASTBinaryFunctionNode(const ASTBinaryFunctionNode& other);
    ASTBinaryFunctionNode& operator = (const ASTBinaryFunctionNode& other);
    ~ASTBinaryFunctionNode();

    virtual std::string toString() const override;
};