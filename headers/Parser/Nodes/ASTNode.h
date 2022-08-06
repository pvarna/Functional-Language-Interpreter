#pragma once

#include "token.h"

struct ASTNode
{
private:
    void copyToken(const Token* token);
    void deallocateToken();

public:
    const Token* token;

    ASTNode(const Token* token);
    ASTNode(const ASTNode& other);
    ASTNode& operator = (const ASTNode& other);
    ~ASTNode();

    virtual std::string toString() const;
};