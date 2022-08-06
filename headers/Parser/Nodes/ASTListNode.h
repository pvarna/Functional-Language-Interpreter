#pragma once

#include "ASTNode.h"
#include <vector>

struct ASTListNode : public ASTNode
{
private:
    void copyElements(const ASTListNode& other);
    void deallocateElements();

public:
    std::vector<const ASTNode*> elements;

    ASTListNode(const Token* token, const std::vector<const ASTNode*>& elements);
    ASTListNode(const ASTListNode& other);
    ASTListNode& operator = (const ASTListNode& other);
    ~ASTListNode();

    virtual std::string toString() const override;
};