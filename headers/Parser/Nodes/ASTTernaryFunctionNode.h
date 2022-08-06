#pragma once

#include "ASTNode.h"

struct ASTTernaryFunctionNode : public ASTNode
{
private:
    void copy(const ASTTernaryFunctionNode& other);
    void deallocate();

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
    virtual ASTTernaryFunctionNode* clone() const override;
};