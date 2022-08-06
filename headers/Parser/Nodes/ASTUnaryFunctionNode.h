#pragma once

#include "ASTNode.h"

struct ASTUnaryFunctionNode : public ASTNode
{
private:
    void copy(const ASTUnaryFunctionNode& other);
    void deallocate();

public:
    const ASTNode* argument;

    ASTUnaryFunctionNode(const Token* token, const ASTNode* argument);
    ASTUnaryFunctionNode(const ASTUnaryFunctionNode& other);
    ASTUnaryFunctionNode& operator = (const ASTUnaryFunctionNode& other);
    ~ASTUnaryFunctionNode();

    virtual std::string toString() const override;
    virtual ASTUnaryFunctionNode* clone() const override;
};