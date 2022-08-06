#pragma once

#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

class ListFunc
{
private:
    Interpreter interpreter;

    ListFunc() = default;

public:
    ListFunc(const ListFunc& other) = delete;
    ListFunc& operator = (const ListFunc& other) = delete;

    static ListFunc& getIstance();

    void run(std::ostream& out);
};