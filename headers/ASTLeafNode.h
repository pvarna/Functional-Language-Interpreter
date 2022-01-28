#pragma once

#include "token.h"

struct ASTLeafNode
{
private:
    void copyToken(const Token* token);
    void deallocateToken();

public:
    const Token* token;

    ASTLeafNode(const Token* token);
    ASTLeafNode(const ASTLeafNode& other);
    ASTLeafNode& operator = (const ASTLeafNode& other);
    ~ASTLeafNode();

    virtual std::string toString() const;
};