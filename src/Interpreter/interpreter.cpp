#include "interpreter.h"
#include "wholeNumberLiteral.h"
#include "wholeNumberToken.h"
#include "fractionalNumberLiteral.h"
#include "fractionalNumberToken.h"
#include "userDefinedListLiteral.h"
#include <iostream>

const double Interpreter::EPS = 0.00001;

void Interpreter::deallocate()
{
    while (!this->visitedLiterals.empty())
    {
        const Literal* current = this->visitedLiterals.top();
        this->visitedLiterals.pop();
        
        delete current;
    }

    for (const Literal* current : this->userArguments)
    {
        delete current;
    }

    this->userArguments.clear();
}

Interpreter::~Interpreter()
{
    this->deallocate();
}

std::string Interpreter::interpret(const ASTNode* astTree, std::ostream& out)
{
    this->deallocate();

    this->visit(astTree);

    if (this->visitedLiterals.empty())
    {
        throw std::runtime_error("Runtime error");
    }

    const Literal* top = this->visitedLiterals.top();

    std::string result = top->toString();

    this->visitedLiterals.pop();
    delete top;

    if (!this->visitedLiterals.empty())
    {
        throw std::runtime_error("Too many arguments");
    }

    return result;
}

void Interpreter::visit(const ASTNode* node)
{
    if (!node->token)
    {
        throw std::runtime_error("Runtime error");
    }

    switch (node->token->type)
    {
    case TokenType::WHOLE_NUMBER:
        this->visitWholeNumber(node);
        break;
    case TokenType::FRACTIONAL_NUMBER:
        this->visitFractionalNumber(node);
        break;
    case TokenType::OPENING_SQUARE_BRACKET:
        this->visitUserDefinedList(dynamic_cast<const ASTListNode*>(node));
        break;
    default:
        throw std::runtime_error("Not implemented yet");
    }
}

void Interpreter::visitWholeNumber(const ASTNode* node)
{
    const WholeNumberToken* token = dynamic_cast<const WholeNumberToken*>(node->token);

    this->visitedLiterals.push(new WholeNumberLiteral(token->value));
}

void Interpreter::visitFractionalNumber(const ASTNode* node)
{
    const FractionalNumberToken* token = dynamic_cast<const FractionalNumberToken*>(node->token);

    this->visitedLiterals.push(new FractionalNumberLiteral(token->value));
}

void Interpreter::visitUserDefinedList(const ASTListNode* node)
{
    std::list<const Literal*> list;

    for (const ASTNode* current : node->elements)
    {
        this->visit(current);
    }

    std::size_t size = node->elements.size();
    for (std::size_t i = 0; i < size; ++i)
    {
        if (this->visitedLiterals.empty())
        {
            throw std::runtime_error("Runtime error");
        }

        const Literal* top = this->visitedLiterals.top();
        list.push_front(top);
        this->visitedLiterals.pop();
    }

    this->visitedLiterals.push(new UserDefinedListLiteral(list));

    for (const Literal* current : list)
    {
        delete current;
    }
    list.clear();
}