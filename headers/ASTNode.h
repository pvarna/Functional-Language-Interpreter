#pragma once

#include "token.h"

class ASTNode
{
private:
    Token* token;

    void copy(const Token* token);
    void deallocate();

public:
    ASTNode(const Token* token);
    ASTNode(const ASTNode& other);
    ASTNode& operator = (const ASTNode& other);
    virtual ~ASTNode();

    virtual std::string toString() const;
};