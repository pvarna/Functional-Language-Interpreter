#pragma once

#include "token.h"

struct ASTNode
{
private:
    void copy(const ASTNode& other);
    void deallocate();

public:
    Token* token;

    ASTNode(const Token* token);
    ASTNode(const ASTNode& other);
    ASTNode& operator = (const ASTNode& other);
    virtual ~ASTNode();

    virtual std::string toString() const;
    virtual ASTNode* clone() const;
};