#pragma once

#include "ASTNode.h"

struct ASTBinaryFunctionNode : public ASTNode
{
private:
    void copy(const ASTBinaryFunctionNode& other);
    void deallocate();

public:
    const ASTNode* firstArgument;
    const ASTNode* secondArgument;

    ASTBinaryFunctionNode(const Token* token, const ASTNode* firstArgument,
                                              const ASTNode* secondArgument);
    ASTBinaryFunctionNode(const ASTBinaryFunctionNode& other);
    ASTBinaryFunctionNode& operator = (const ASTBinaryFunctionNode& other);
    ~ASTBinaryFunctionNode();

    virtual std::string toString() const override;
    virtual ASTBinaryFunctionNode* clone() const override;
};