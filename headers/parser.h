#pragma once

#include <vector>
#include "token.h"
#include "ASTNode.h"

class Parser
{
private:
    std::string text;
    std::vector<Token*> tokens;
    int currentIndex;

    ASTNode* number();
    ASTNode* list();
    ASTNode* function(); // not allowing arguments like #0, #1, etc.

public:
    Parser(const std::string& text, const std::vector<Token*>& tokens);
    Parser(const Parser& other) = delete;
    Parser operator = (const Parser& other) = delete;
    ~Parser();

    ASTNode* parse();
};