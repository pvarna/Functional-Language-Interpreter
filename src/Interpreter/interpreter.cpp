#include "interpreter.h"
#include "utils.h"
#include "wholeNumberLiteral.h"
#include "wholeNumberToken.h"
#include "argumentToken.h"
#include "fractionalNumberLiteral.h"
#include "fractionalNumberToken.h"
#include "userDefinedListLiteral.h"
#include "functionNameToken.h"
#include "infiniteListLiteral.h"
#include "concatenatedListLiteral.h"
#include "finiteListLiteral.h"
#include <iostream>
#include <cmath>

const std::unordered_set<std::string> Interpreter::SAVED_FUNCTIONS = 
{
    "eq", "le", "nand", "length", "head", "tail", "list", "concat",
    "if", "read", "write", "int", "add", "sub", "mul", "div", "mod", "sqrt"
};

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

    this->offset = 0;
}

Interpreter::Interpreter() : offset(0) {}

Interpreter::~Interpreter()
{
    for (std::pair<std::string, const ASTNode*> current : this->userFunctions)
    {
        delete current.second;
    }
    this->deallocate();
}

std::string Interpreter::interpret(const ASTNode* astTree)
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
    case TokenType::ARGUMENT:
        this->visitArgument(node);
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
        this->visitUnaryFunction(dynamic_cast<const ASTUnaryFunctionNode*>(node));
    }
    else if (typeid(*node) == typeid(ASTBinaryFunctionNode))
    {
        this->visitBinaryFunction(dynamic_cast<const ASTBinaryFunctionNode*>(node));
    }
    else if (typeid(*node) == typeid(ASTTernaryFunctionNode))
    {
        this->visitTernaryFunction(dynamic_cast<const ASTTernaryFunctionNode*>(node));
    }
    else if (typeid(*node) == typeid(ASTN_aryFunctionNode))
    {
        this->visitN_aryFunction(dynamic_cast<const ASTN_aryFunctionNode*>(node));
    }
    else if (typeid(*node) == typeid(ASTUserFunctionNode))
    {
        this->visitUserFunction(dynamic_cast<const ASTUserFunctionNode*>(node));
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
        throw std::runtime_error("Unknown function name: " + functionName);
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

    std::string functionName = token->name;

    if (this->userFunctions.count(functionName) != 0)
    {
        this->visit(node->argument);

        int difference = this->userArguments.size() - this->offset;
        this->offset = this->userArguments.size();

        const Literal* top = this->visitedLiterals.top();
        this->visitedLiterals.pop();

        this->userArguments.push_back(top->clone());
        const ASTNode* definition = this->userFunctions.at(functionName);

        this->visit(definition);
        delete top;

        this->offset -= difference;
        const Literal* back = this->userArguments.back();
        this->userArguments.pop_back();
        delete back;
    }
    else if (functionName == "sqrt")
    {
        this->sqrt(node);
    }
    else if (functionName == "head")
    {
        this->head(node);
    }
    else if (functionName == "tail")
    {
        this->tail(node);
    }
    else if (functionName == "length")
    {
        this->length(node);
    }
    else if (functionName == "list")
    {
        this->list(node);
    }
    else if (functionName == "write")
    {
        this->write(node);
    }
    else if (functionName == "int")
    {   
        this->toInt(node);
    }
    else
    {
        throw std::runtime_error("Unknown function name: " + functionName);
    }
}

void Interpreter::sqrt(const ASTUnaryFunctionNode* node)
{
    this->visit(node->argument);

    const Literal* argument = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    double result;

    try
    {
        result = Literal::valueOf(argument);
    }
    catch(...)
    {
        delete argument;
        throw std::runtime_error("Expected a number for the \'sqrt\' function");
    }

    if (result < 0)
    {
        delete argument;
        throw std::runtime_error("Expected a non-negative number for the \'sqrt\' function");
    }

    this->visitedLiterals.push(Literal::of(std::sqrt(result)));
    delete argument;
}

void Interpreter::head(const ASTUnaryFunctionNode* node)
{
    if (node->argument->token->type == TokenType::OPENING_SQUARE_BRACKET)
    {
        const ASTListNode* listNode = dynamic_cast<const ASTListNode*>(node->argument);
        if (listNode->elements.empty())
        {
            throw std::runtime_error("Expected non-empty list for the \'head\' function");
        }

        this->visit(listNode->elements.front());
        return;
    }

    this->visit(node->argument);

    const Literal* argument = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    const ListLiteral* listLiteral = dynamic_cast<const ListLiteral*>(argument);

    if (!listLiteral)
    {
        delete argument;
        throw std::runtime_error("Expected a list for the \'head\' function");
    }

    Literal* toAdd = listLiteral->head();
    if (!toAdd)
    {
        delete argument;
        throw std::runtime_error("Expected non-empty list for the \'head\' function");
    }

    this->visitedLiterals.push(toAdd);
    delete argument;
}

void Interpreter::tail(const ASTUnaryFunctionNode* node)
{
    this->visit(node->argument);

    const Literal* argument = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    const ListLiteral* listLiteral = dynamic_cast<const ListLiteral*>(argument);

    if (!listLiteral)
    {
        delete argument;
        throw std::runtime_error("Expected a list for the \'tail\' function");
    }

    ListLiteral* toAdd = listLiteral->tail();
    if (!toAdd)
    {
        delete argument;
        throw std::runtime_error("Expected non-empty list for the \'tail\' function");
    }

    this->visitedLiterals.push(toAdd); 
    delete argument;
}

void Interpreter::length(const ASTUnaryFunctionNode* node)
{
    this->visit(node->argument);

    const Literal* argument = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    int length;

    try
    {
        length = argument->length();
    }
    catch(...)
    {
        delete argument;
        throw;
    }
    
    this->visitedLiterals.push(new WholeNumberLiteral(length));
    delete argument;   
}

void Interpreter::list(const ASTUnaryFunctionNode* node)
{
    this->visit(node->argument);

    const Literal* argument = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    
    double firstElement;

    try
    {
        firstElement = Literal::valueOf(argument);
    }
    catch(...)
    {
        delete argument;
        throw std::runtime_error("Expected a number for the \'list\' function");
    }

    this->visitedLiterals.push(new InfiniteListLiteral(firstElement));
    delete argument;
}

void Interpreter::write(const ASTUnaryFunctionNode* node)
{
    try
    {
        this->visit(node->argument);
    }
    catch(...)
    {
        this->visitedLiterals.push(new WholeNumberLiteral(-1));
        return;
    }

    const Literal* argument = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    std::cout << argument->toString() << std::endl;
    this->visitedLiterals.push(new WholeNumberLiteral(0));
    delete argument;    
}

void Interpreter::toInt(const ASTUnaryFunctionNode* node)
{
    this->visit(node->argument);

    const Literal* argument = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    double result;

    try
    {
        result = Literal::valueOf(argument);
    }
    catch(...)
    {
        delete argument;
        throw std::runtime_error("Expected a number for the \'int\' function");
    }

    this->visitedLiterals.push(Literal::of(static_cast<int>(result)));
    delete argument;
}

void Interpreter::visitBinaryFunction(const ASTBinaryFunctionNode* node)
{
    const FunctionNameToken* token = dynamic_cast<const FunctionNameToken*>(node->token);

    std::string functionName = token->name;

    if (this->userFunctions.count(functionName) != 0)
    {
        this->visit(node->firstArgument);
        this->visit(node->secondArgument);

        int difference = this->userArguments.size() - this->offset;
        this->offset = this->userArguments.size();

        const Literal* second = this->visitedLiterals.top();
        this->visitedLiterals.pop();
        const Literal* first = this->visitedLiterals.top();
        this->visitedLiterals.pop();

        this->userArguments.push_back(first->clone());
        this->userArguments.push_back(second->clone());
        const ASTNode* definition = this->userFunctions.at(functionName);

        this->visit(definition);
        delete first;
        delete second;

        this->offset -= difference;
        const Literal* back = this->userArguments.back();
        this->userArguments.pop_back();
        delete back;
        back = this->userArguments.back();
        this->userArguments.pop_back();
        delete back; 
    }
    else if (functionName == "concat")
    {
        this->concat(node);
    }
    else if (functionName == "eq")
    {
        this->eq(node);
    }
    else if (functionName == "le")
    {
        this->le(node);
    }
    else if (functionName == "nand")
    {
        this->nand(node);
    }
    else if (functionName == "list")
    {
        this->list(node);
    }
    else if (functionName == "add" || functionName == "sub" ||
             functionName == "mul" || functionName == "div" ||
             functionName == "mod")
    {
        this->binaryArithmethic(node, functionName);
    }
    else
    {
        throw std::runtime_error("Unknown function name: " + functionName);
    }
}

void Interpreter::concat(const ASTBinaryFunctionNode* node)
{
    this->visit(node->firstArgument);

    const Literal* firstArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    const ListLiteral* firstList = dynamic_cast<const ListLiteral*>(firstArgument);

    if (!firstList)
    {
        delete firstArgument;
        throw std::runtime_error("Expected two lists for the \'concat\' function");
    }

    if (firstList->isInfinite())
    {
        delete firstArgument;
        throw std::runtime_error("Expected a finite list as a first parameter for the \'concat\' function");
    }

    this->visit(node->secondArgument);

    const Literal* secondArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    const ListLiteral* secondList = dynamic_cast<const ListLiteral*>(secondArgument);

    if (!secondList)
    {
        delete firstArgument;
        delete secondArgument;
        throw std::runtime_error("Expected two lists for the \'concat\' function");
    }

    this->visitedLiterals.push(new ConcatenatedListLiteral(firstList, secondList));
    delete firstList;
    delete secondList;
}

void Interpreter::eq(const ASTBinaryFunctionNode* node)
{
    this->visit(node->firstArgument);
    this->visit(node->secondArgument);

    const Literal* second = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    const Literal* first = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    this->visitedLiterals.push(new WholeNumberLiteral(Literal::eq(first, second)));
    delete first;
    delete second;
}

void Interpreter::le(const ASTBinaryFunctionNode* node)
{
    this->visit(node->firstArgument);

    const Literal* firstArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    double firstNumber;

    try
    {
        firstNumber = Literal::valueOf(firstArgument);
    }
    catch(...)
    {
        delete firstArgument;
        throw std::runtime_error("Expected two numbers for the \'le\' function");
    }

    this->visit(node->secondArgument);

    const Literal* secondArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    double secondNumber;

    try
    {
        secondNumber = Literal::valueOf(secondArgument);
    }
    catch(...)
    {
        delete firstArgument;
        delete secondArgument;
        throw std::runtime_error("Expected two numbers for the \'le\' function");
    }

    this->visitedLiterals.push(new WholeNumberLiteral(firstNumber < secondNumber)); 
    delete firstArgument;
    delete secondArgument;
}

void Interpreter::nand(const ASTBinaryFunctionNode* node)
{
    this->visit(node->firstArgument);

    const Literal* firstArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    if (!firstArgument->toBool())
    {
        delete firstArgument;
        this->visitedLiterals.push(new WholeNumberLiteral(true));
        return;
    }

    this->visit(node->secondArgument);

    const Literal* secondArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    this->visitedLiterals.push(new WholeNumberLiteral(!firstArgument->toBool() || !secondArgument->toBool()));
    delete firstArgument;
    delete secondArgument;
}

void Interpreter::list(const ASTBinaryFunctionNode* node)
{
    this->visit(node->firstArgument);

    const Literal* firstArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    double firstNumber;

    try
    {
        firstNumber = Literal::valueOf(firstArgument);
    }
    catch(...)
    {
        delete firstArgument;
        throw std::runtime_error("Expected two numbers for the \'list\' function");
    }

    this->visit(node->secondArgument);

    const Literal* secondArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    double secondNumber;

    try
    {
        secondNumber = Literal::valueOf(secondArgument);
    }
    catch(...)
    {
        delete firstArgument;
        delete secondArgument;
        throw std::runtime_error("Expected two numbers for the \'list\' function");
    } 

    this->visitedLiterals.push(new InfiniteListLiteral(firstNumber, secondNumber));
}

void Interpreter::binaryArithmethic(const ASTBinaryFunctionNode* node, const std::string& functionName)
{
    this->visit(node->firstArgument);

    const Literal* firstArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    double firstNumber;

    try
    {
        firstNumber = Literal::valueOf(firstArgument);
    }
    catch(...)
    {
        delete firstArgument;
        throw std::runtime_error("Expected two numbers for the \'" + functionName +"\' function");
    }

    if (functionName == "mod" && firstArgument->type != LiteralType::WHOLE_NUMBER)
    {
        delete firstArgument;
        throw std::runtime_error("Expected two whole numbers for the \'" + functionName +"\' function");
    }

    this->visit(node->secondArgument);

    const Literal* secondArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    double secondNumber;

    try
    {
        secondNumber = Literal::valueOf(secondArgument);
    }
    catch(...)
    {
        delete firstArgument;
        delete secondArgument;
        throw std::runtime_error("Expected two numbers for the \'" + functionName +"\' function");
    }

    if (functionName == "mod" && secondArgument->type != LiteralType::WHOLE_NUMBER)
    {
        delete firstArgument;
        delete secondArgument;
        throw std::runtime_error("Expected two whole numbers for the \'" + functionName +"\' function");
    }

    if (functionName == "mod")
    {
        int result = static_cast<int>(firstNumber) % static_cast<int>(secondNumber);

        this->visitedLiterals.push(new WholeNumberLiteral(result));
        delete firstArgument;
        delete secondArgument;
        return; 
    }

    double result;
    if (functionName == "add")
    {
        result = firstNumber + secondNumber;
    }
    else if (functionName == "sub")
    {
        result = firstNumber - secondNumber;
    }
    else if (functionName == "mul")
    {
        result = firstNumber * secondNumber;
    }
    else if (functionName == "div")
    {
        if (std::abs(secondNumber - 0) < Utils::EPS)
        {
            throw std::runtime_error("Cannot divide by 0");
        }
        result = firstNumber / secondNumber;
    }

    this->visitedLiterals.push(Literal::of(result));
    delete firstArgument;
    delete secondArgument;
}

void Interpreter::visitTernaryFunction(const ASTTernaryFunctionNode* node)
{
    const FunctionNameToken* token = dynamic_cast<const FunctionNameToken*>(node->token);

    std::string functionName = token->name;

    if (this->userFunctions.count(functionName) != 0)
    {
        this->visit(node->firstArgument);
        this->visit(node->secondArgument);
        this->visit(node->thirdArgument);

        int difference = this->userArguments.size() - this->offset;
        this->offset = this->userArguments.size();

        const Literal* third = this->visitedLiterals.top();
        this->visitedLiterals.pop();
        const Literal* second = this->visitedLiterals.top();
        this->visitedLiterals.pop();
        const Literal* first = this->visitedLiterals.top();
        this->visitedLiterals.pop();

        this->userArguments.push_back(first->clone());
        this->userArguments.push_back(second->clone());
        this->userArguments.push_back(third->clone());
        this->visit(this->userFunctions[functionName]);
        delete first;
        delete second;
        delete third;

        this->offset -= difference;
        const Literal* back = this->userArguments.back();
        this->userArguments.pop_back();
        delete back;
        back = this->userArguments.back();
        this->userArguments.pop_back();
        delete back; 
        back = this->userArguments.back();
        this->userArguments.pop_back();
        delete back; 
    }
    else if (functionName == "list")
    {
        this->list(node);
    }
    else if (functionName == "if")
    {
        this->visitIf(node);
    }
    else
    {
        throw std::runtime_error("Unknown function name: " + functionName);
    }
}

void Interpreter::list(const ASTTernaryFunctionNode* node)
{
    this->visit(node->firstArgument);

    const Literal* firstArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    double firstNumber;

    try
    {
        firstNumber = Literal::valueOf(firstArgument);
    }
    catch(...)
    {
        delete firstArgument;
        throw std::runtime_error("Expected three numbers for the \'list\' function");
    }

    this->visit(node->secondArgument);

    const Literal* secondArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    double secondNumber;

    try
    {
        secondNumber = Literal::valueOf(secondArgument);
    }
    catch(...)
    {
        delete firstArgument;
        delete secondArgument;
        throw std::runtime_error("Expected three numbers for the \'list\' function");
    }

    this->visit(node->thirdArgument);

    const Literal* thirdArgument = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    double thirdNumber;

    try
    {
        thirdNumber = Literal::valueOf(thirdArgument);
    }
    catch(...)
    {
        delete firstArgument;
        delete secondArgument;
        delete thirdArgument;
        throw std::runtime_error("Expected three numbers for the \'list\' function");
    }

    if (thirdArgument->type != LiteralType::WHOLE_NUMBER)
    {
        delete firstArgument;
        delete secondArgument;
        delete thirdArgument;
        throw std::runtime_error("Expected a whole number as a third parameterfor the \'list\' function");
    }

    this->visitedLiterals.push(new FiniteListLiteral(firstNumber, secondNumber, thirdNumber));
    delete firstArgument;
    delete secondArgument;
    delete thirdArgument;
}

void Interpreter::visitIf(const ASTTernaryFunctionNode* node)
{
    this->visit(node->firstArgument);

    const Literal* condition = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    bool result = condition->toBool();
    delete condition;

    result ? this->visit(node->secondArgument) : 
             this->visit(node->thirdArgument);
}

void Interpreter::visitN_aryFunction(const ASTN_aryFunctionNode* node)
{
    const FunctionNameToken* token = dynamic_cast<const FunctionNameToken*>(node->token);

    std::string functionName = token->name;

    if (this->userFunctions.count(functionName) != 0)
    {
        std::vector<const Literal*> arguments;

        for (const ASTNode* current : node->arguments)
        {
            try
            {
                this->visit(current);

                const Literal* top = this->visitedLiterals.top();
                this->visitedLiterals.pop();
                arguments.push_back(top);
                this->userArguments.push_back(top->clone());
            }
            catch(...)
            {
                for (const Literal* literal : arguments)
                {
                    delete literal;
                }
                arguments.clear();
                throw;
            }   
        }

        int difference = this->userArguments.size() - node->arguments.size() - this->offset;
        this->offset = this->userArguments.size() - node->arguments.size();

        this->visit(this->userFunctions[functionName]);

        int argumentsSize = arguments.size();
        for (const Literal* literal : arguments)
        {
            delete literal;
        }
        arguments.clear();

        for (int i = 0; i < argumentsSize; ++i)
        {
            const Literal* back = this->userArguments.back();
            this->userArguments.pop_back();
            delete back;
        }
    }
    else
    {
        throw std::runtime_error("Unknown function name: " + functionName);
    }
}

void Interpreter::visitUserFunction(const ASTUserFunctionNode* node)
{
    const FunctionNameToken* token = dynamic_cast<const FunctionNameToken*>(node->token);

    if (Interpreter::SAVED_FUNCTIONS.count(token->name) != 0)
    {
        throw std::runtime_error("Cannot declare function with the saved name \'"  + token->name + "\'");
    }

    int result = 0;
    if (this->userFunctions.count(token->name) != 0)
    {
        delete this->userFunctions[token->name];
        result = 1;
    }

    this->userFunctions[token->name] = node->definition->clone();

    this->visitedLiterals.push(new WholeNumberLiteral(result));
}

void Interpreter::visitArgument(const ASTNode* node)
{
    const ArgumentToken* token = dynamic_cast<const ArgumentToken*>(node->token);

    if (token->index >= this->userArguments.size())
    {
        std::runtime_error("Too few arguments");
    }

    const Literal* toCopy = this->userArguments[token->index];

    this->visitedLiterals.push(this->userArguments[token->index + this->offset]->clone());     
}