#include "interpreter.h"
#include "utils.h"
#include "wholeNumberLiteral.h"
#include "wholeNumberToken.h"
#include "fractionalNumberLiteral.h"
#include "fractionalNumberToken.h"
#include "userDefinedListLiteral.h"
#include "functionNameToken.h"
#include "infiniteListLiteral.h"
#include <iostream>
#include <cmath>

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

    try
    {
        this->visit(astTree);
    }
    catch(...)
    {
        this->deallocate();
        throw;
    }

    if (this->visitedLiterals.empty())
    {
        this->deallocate();
        throw std::runtime_error("Runtime error");
    }

    const Literal* top = this->visitedLiterals.top();

    std::string result = top->toString();

    this->visitedLiterals.pop();
    delete top;

    if (!this->visitedLiterals.empty())
    {
        this->deallocate();
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
    case TokenType::FUNCTION_NAME:
        this->visitFunction(node);
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

void Interpreter::visitFunction(const ASTNode* node)
{
    if (typeid(*node) == typeid(ASTNode))
    {
        this->visitFunctionWithoutArguments(node);
    }
    else if (typeid(*node) == typeid(ASTUnaryFunctionNode))
    {
        const ASTUnaryFunctionNode* unaryFunctionNode = dynamic_cast<const ASTUnaryFunctionNode*>(node);
        const ASTNode* argument = unaryFunctionNode->argument;

        this->visit(argument);
        this->visitUnaryFunction(unaryFunctionNode);
    }
}

void Interpreter::visitFunctionWithoutArguments(const ASTNode* node)
{
    const FunctionNameToken* token = dynamic_cast<const FunctionNameToken*>(node->token);

    std::string functionName = token->name;

    if (this->userFunctions.count(functionName) != 0)
    {
        this->visit(this->userFunctions[functionName]);
    }
    else if (functionName == "read")
    {
        this->read();
    }
    else
    {
        throw std::runtime_error("Unknown function name");
    }
}

void Interpreter::read()
{
    double number;

    std::cout << "read(): ";
    std::cin >> number;

    while (!std::cin)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "A number should be entered" << std::endl;
        std::cout << "read(): ";
        std::cin >> number;
    }

    std::cin.ignore();

    this->visitedLiterals.push(Literal::of(number));                    
}

void Interpreter::visitUnaryFunction(const ASTUnaryFunctionNode* node)
{
    const FunctionNameToken* token = dynamic_cast<const FunctionNameToken*>(node->token);

    const Literal* top = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    std::string functionName = token->name;

    try
    {
        if (this->userFunctions.count(functionName) != 0)
        {
            this->userArguments.push_back(top->clone());
            this->visit(this->userFunctions[functionName]);
        }
        else if (functionName == "sqrt")
        {
            this->sqrt(top);
        }
        else if (functionName == "head")
        {
            this->head(top);
        }
        else if (functionName == "tail")
        {
            this->tail(top);
        }
        else if (functionName == "length")
        {
            this->length(top);
        }
        else if (functionName == "list")
        {
            this->list(top);
        }
    }
    catch(...)
    {
       delete top;
       throw;
    }

    delete top;
}

void Interpreter::sqrt(const Literal* argument)
{
    double result;

    switch (argument->type)
    {
    case LiteralType::WHOLE_NUMBER:
        result = dynamic_cast<const WholeNumberLiteral*>(argument)->value;
        break;
    case LiteralType::FRACTIONAL_NUMBER:
        result = dynamic_cast<const FractionalNumberLiteral*>(argument)->value;
        break;
    default:
        throw std::runtime_error("Expected a number for the \'sqrt\' function");
    }

    if (result < 0)
    {
        throw std::runtime_error("Expected a non-negative number for the \'sqrt\' function");
    }

    this->visitedLiterals.push(Literal::of(std::sqrt(result)));
}

void Interpreter::head(const Literal* argument)
{
    const ListLiteral* listLiteral = dynamic_cast<const ListLiteral*>(argument);

    if (!listLiteral)
    {
        throw std::runtime_error("Expected a list for the \'head\' function");
    }

    Literal* toAdd = listLiteral->head();
    if (!toAdd)
    {
        throw std::runtime_error("Expected non-empty list for the \'head\' function");
    }

    this->visitedLiterals.push(toAdd);
}

void Interpreter::tail(const Literal* argument)
{
    const ListLiteral* listLiteral = dynamic_cast<const ListLiteral*>(argument);

    if (!listLiteral)
    {
        throw std::runtime_error("Expected a list for the \'tail\' function");
    }

    ListLiteral* toAdd = listLiteral->tail();
    if (!toAdd)
    {
        throw std::runtime_error("Expected non-empty list for the \'tail\' function");
    }

    this->visitedLiterals.push(toAdd); 
}

void Interpreter::length(const Literal* argument)
{
    int length;

    try
    {
        length = argument->length();
    }
    catch(...)
    {
        throw std::runtime_error("Not expected infinite list for the \'length\' function");
    }

    this->visitedLiterals.push(new WholeNumberLiteral(length));   
}

void Interpreter::list(const Literal* argument)
{
    double firstElement;

    switch (argument->type)
    {
    case LiteralType::WHOLE_NUMBER:
        firstElement = dynamic_cast<const WholeNumberLiteral*>(argument)->value;
        break;
    case LiteralType::FRACTIONAL_NUMBER:
        firstElement = dynamic_cast<const FractionalNumberLiteral*>(argument)->value;
        break;
    default:
        throw std::runtime_error("Expected a number for the \'list\' function");
    }

    this->visitedLiterals.push(new InfiniteListLiteral(firstElement));
}