#pragma once

#include <vector>
#include "token.h"
#include "ASTLeafNode.h"

class Parser
{
private:
    std::vector<Token*> tokens;
    int currentIndex;

public:
    Parser(const std::vector<Token*>& tokens);
    Parser(const Parser& other) = delete;
    Parser operator = (const Parser& other) = delete;
    ~Parser();

    ASTLeafNode* parse();
};