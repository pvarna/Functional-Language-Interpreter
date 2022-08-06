#pragma once

#include "ASTNode.h"
#include <vector>

struct ASTN_aryFunctionNode : public ASTNode
{
private:
    void copyArguments(const ASTN_aryFunctionNode& other);
    void deallocateArguments();

public:
    std::vector<const ASTNode*> arguments;

    ASTN_aryFunctionNode(const Token* token, const std::vector<const ASTNode*>& arguments);
    ASTN_aryFunctionNode(const ASTN_aryFunctionNode& other);
    ASTN_aryFunctionNode& operator = (const ASTN_aryFunctionNode& other);
    ~ASTN_aryFunctionNode();

    virtual std::string toString() const override;
};