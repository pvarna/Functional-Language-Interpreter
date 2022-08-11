#pragma once

#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

class ListFunc
{
private:
    void deallocateResources(std::vector<Token*>& tokens, ASTNode* tree = nullptr);

    Interpreter interpreter;

    ListFunc() = default;

public:
    ListFunc(const ListFunc& other) = delete;
    ListFunc& operator = (const ListFunc& other) = delete;
    ~ListFunc() = default;

    static ListFunc& getIstance();

    void run();
};