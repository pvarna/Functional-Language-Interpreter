#pragma once

#include "ASTNode.h"

struct ASTTernaryFunctionNode : public ASTNode
{
private:
    void copyArguments(const ASTTernaryFunctionNode& other);
    void deallocateArguments();

public:
    const ASTNode* firstArgument;
    const ASTNode* secondArgument;
    const ASTNode* thirdArgument;

    ASTTernaryFunctionNode(const Token* token, const ASTNode* firstArgument,
                        const ASTNode* secondArgument, const ASTNode* thirdArgument);
    ASTTernaryFunctionNode(const ASTTernaryFunctionNode& other);
    ASTTernaryFunctionNode& operator = (const ASTTernaryFunctionNode& other);
    ~ASTTernaryFunctionNode();

    virtual std::string toString() const override;
};