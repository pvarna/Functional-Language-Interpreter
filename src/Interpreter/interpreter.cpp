#include "interpreter.h"
#include "wholeNumberToken.h"
#include "wholeNumberLiteral.h"
#include "fractionalNumberToken.h"
#include "fractionalNumberLiteral.h"
#include "userDefinedListLiteral.h"
#include "ASTListNode.h"
#include "functionNameToken.h"
#include "ASTUnaryFunctionNode.h"
#include "functionDefinedListLiteral.h"
#include "argumentToken.h"
#include <cmath>
#include <iostream>
#include <limits>

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

const Literal* Interpreter::copy(const Literal* toCopy)
{
    const Literal* copy = nullptr; 
    if (toCopy->type == LiteralType::WHOLE_NUMBER)
    {
        copy = new WholeNumberLiteral(dynamic_cast<const WholeNumberLiteral*>(toCopy)->value);
    }
    else if (toCopy->type == LiteralType::FRACTIONAL_NUMBER)
    {
        copy = new FractionalNumberLiteral(dynamic_cast<const FractionalNumberLiteral*>(toCopy)->value);
    } 
    else if (toCopy->type == LiteralType::USER_DEFINED_LIST)
    {
        copy = new UserDefinedListLiteral(dynamic_cast<const UserDefinedListLiteral*>(toCopy)->elements);
    }
    else
    {
        const FunctionDefinedListLiteral* functionDefined = dynamic_cast<const FunctionDefinedListLiteral*>(toCopy);
        copy = new FunctionDefinedListLiteral(functionDefined->firstElement, functionDefined->step, functionDefined->numberOfElements);
    }

    return copy;
}

void Interpreter::addToUserFunctions(const std::string& name, const ASTNode* definition)
{
    if (typeid(*definition) == typeid(ASTNode))
    {
        this->userFunctions.insert({name, new ASTNode(*definition)});
    }
    else if (typeid(*definition) == typeid(ASTUnaryFunctionNode))
    {
        const ASTUnaryFunctionNode* unary = dynamic_cast<const ASTUnaryFunctionNode*>(definition);
        this->userFunctions.insert({name, new ASTUnaryFunctionNode(*unary)});
    }
    else if (typeid(*definition) == typeid(ASTBinaryFunctionNode))
    {
        const ASTBinaryFunctionNode* binary = dynamic_cast<const ASTBinaryFunctionNode*>(definition);
        this->userFunctions.insert({name, new ASTBinaryFunctionNode(*binary)});
    }
    else if (typeid(*definition) == typeid(ASTUnaryFunctionNode))
    {
        const ASTTernaryFunctionNode* ternary = dynamic_cast<const ASTTernaryFunctionNode*>(definition);
        this->userFunctions.insert({name, new ASTTernaryFunctionNode(*ternary)});
    }
    else if (typeid(*definition) == typeid(ASTListNode))
    {
        const ASTListNode* list = dynamic_cast<const ASTListNode*>(definition);
        this->userFunctions.insert({name, new ASTListNode(*list)});
    }
}

bool Interpreter::isInt(double value)
{
    return floor(value) == ceil(value);
}

void Interpreter::visitNumber(const ASTNode* node)
{
    const Token* token = node->token;

    if (token->type == TokenType::WHOLE_NUMBER)
    {
        const WholeNumberToken* wholeNumberToken = dynamic_cast<const WholeNumberToken*>(token);

        this->visitedLiterals.push(new WholeNumberLiteral(wholeNumberToken->value));
    }
    else
    {
        const FractionalNumberToken* fractionalNumberToken = dynamic_cast<const FractionalNumberToken*>(token);

        this->visitedLiterals.push(new FractionalNumberLiteral(fractionalNumberToken->value));
    }
}

void Interpreter::visitList(const ASTListNode* node)
{
    std::list<double> values;

    for (const ASTNode* current : node->elements)
    {
        const Token* token = current->token;

        if (token->type == TokenType::WHOLE_NUMBER)
        {
            const WholeNumberToken* wholeNumberToken = dynamic_cast<const WholeNumberToken*>(token);

            values.push_back(wholeNumberToken->value);
        }
        else
        {
            const FractionalNumberToken* fractionalNumberToken = dynamic_cast<const FractionalNumberToken*>(token);

            values.push_back(fractionalNumberToken->value);
        }
    }

    this->visitedLiterals.push(new UserDefinedListLiteral(values));
}

double Interpreter::getNumber(const Literal* number)
{
    if (number->type == LiteralType::WHOLE_NUMBER)
    {
        return dynamic_cast<const WholeNumberLiteral*>(number)->value;
    }

    return dynamic_cast<const FractionalNumberLiteral*>(number)->value;
}

std::list<double> Interpreter::getList(const Literal* list)
{
    std::list<double> result;
    if (list->type == LiteralType::USER_DEFINED_LIST)
    {
        const UserDefinedListLiteral* userList = dynamic_cast<const UserDefinedListLiteral*>(list);
        result = userList->elements;
    }
    else
    {
        const FunctionDefinedListLiteral* functionList = dynamic_cast<const FunctionDefinedListLiteral*>(list);
        double currentElement = functionList->firstElement;
        for (int i = 0; i < functionList->numberOfElements; ++i)
        {
            result.push_back(currentElement);

            currentElement += functionList->step;
        }
    }

    return result;
}

bool Interpreter::compareNumberAndList(const Literal* number, const Literal* list)
{
    double valueNumber = this->getNumber(number);
    std::list<double> valueList = this->getList(list);

    return valueList.size() == 1 && fabs(valueNumber - valueList.front() < Interpreter::EPS);
}

bool Interpreter::compareLists(const Literal* list1, const Literal* list2)
{
    if (list1->type == LiteralType::FUNCTION_DEFINED_LIST && list1->type == list2->type)
    {
        const FunctionDefinedListLiteral* l1 = dynamic_cast<const FunctionDefinedListLiteral*>(list1);
        const FunctionDefinedListLiteral* l2 = dynamic_cast<const FunctionDefinedListLiteral*>(list2);

        return l1->isInfinite == l2->isInfinite && fabs(l1->firstElement - l2->firstElement) < Interpreter::EPS 
            && fabs(l1->step - l2->step) < Interpreter::EPS && l1->numberOfElements == l2->numberOfElements;
    }

    std::list<double> firstList = this->getList(list1);
    std::list<double> secondList = this->getList(list2);

    return firstList == secondList;
}

bool Interpreter::isInfiniteList(const Literal* list)
{
    return list->type == LiteralType::FUNCTION_DEFINED_LIST && dynamic_cast<const FunctionDefinedListLiteral*>(list)->isInfinite;
}

void Interpreter::visitFunctionWithoutArguments(const ASTNode* node)
{
    const FunctionNameToken* token = dynamic_cast<const FunctionNameToken*>(node->token);

    if (this->userFunctions.find(token->name) != this->userFunctions.end())
    {
        this->visit(this->userFunctions[token->name]);
    }
    else if (token->name == "read")
    {
        double number;
        
        std::cout << "read(): ";
        std::cin >> number;
        if (!std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("A number should be entered");
        }

        std::cin.ignore();

        const Literal* toAdd = nullptr;
        if(this->isInt(number))
        {
            toAdd = new WholeNumberLiteral(number);
        } 
        else
        {
            toAdd = new FractionalNumberLiteral(number);
        }

        this->visitedLiterals.push(toAdd);
    }
    else
    {
        throw std::runtime_error("Unknown function name");
    }
}

void Interpreter::visitUnaryFunction(const ASTUnaryFunctionNode* node)
{
    const FunctionNameToken* token = dynamic_cast<const FunctionNameToken*>(node->token);

    const Literal* top = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    if (this->userFunctions.find(token->name) != this->userFunctions.end())
    {
        this->userArguments.push_back(this->copy(top));

        this->visit(this->userFunctions[token->name]);
    }
    else if (token->name == "sqrt")
    {
        double value;

        if (top->isList())
        {
            throw std::runtime_error("Expected a number in the \'sqrt\' function");
        }
        else
        {
            value = this->getNumber(top);
        }

        double result = sqrt(value);

        const Literal* toAdd = nullptr;
        if(this->isInt(result))
        {
            toAdd = new WholeNumberLiteral(result);
        } 
        else
        {
            toAdd = new FractionalNumberLiteral(result);
        }

        this->visitedLiterals.push(toAdd);
    }
    else if (token->name == "head")
    {
        if (top->isNumber())
        {
            throw std::runtime_error("Expected a list in the \'head\' function");
        }
        else if (top->type == LiteralType::USER_DEFINED_LIST)
        {
            const UserDefinedListLiteral* listLiteral = dynamic_cast<const UserDefinedListLiteral*>(top);
        
            if (listLiteral->elements.empty())
            {
                throw std::runtime_error("The list is empty");
            }

            double result = listLiteral->elements.front();

            const Literal* toAdd = nullptr;
            if(this->isInt(result))
            {
                toAdd = new WholeNumberLiteral(result);
            } 
            else
            {
                toAdd = new FractionalNumberLiteral(result);
            }

            this->visitedLiterals.push(toAdd);
        }
        else
        {
            const FunctionDefinedListLiteral* listLiteral = dynamic_cast<const FunctionDefinedListLiteral*>(top);

            double result = listLiteral->firstElement;

            const Literal* toAdd = nullptr;
            if(this->isInt(result))
            {
                toAdd = new WholeNumberLiteral(result);
            } 
            else
            {
                toAdd = new FractionalNumberLiteral(result);
            }

            this->visitedLiterals.push(toAdd);
        }
    }
    else if (token->name == "tail")
    {
        if (top->isNumber())
        {
            throw std::runtime_error("Expected a list");
        }
        else if (top->type == LiteralType::USER_DEFINED_LIST)
        {
            const UserDefinedListLiteral* listLiteral = dynamic_cast<const UserDefinedListLiteral*>(top);
        
            if (listLiteral->elements.empty())
            {
                throw std::runtime_error("The list is empty");
            }

            std::list<double>::const_iterator start = listLiteral->elements.begin();
            std::list<double>::const_iterator end = listLiteral->elements.end();

            this->visitedLiterals.push(new UserDefinedListLiteral(std::list<double>(++start, end)));
        }
        else
        {
            const FunctionDefinedListLiteral* listLiteral = dynamic_cast<const FunctionDefinedListLiteral*>(top);

            this->visitedLiterals.push(new FunctionDefinedListLiteral(listLiteral->firstElement + listLiteral->step,
                            listLiteral->step, listLiteral->numberOfElements - 1));
        }
    }
    else if (token->name == "length")
    {
        if (top->isNumber())
        {
            this->visitedLiterals.push(new WholeNumberLiteral(-1));
        }
        else if (top->type == LiteralType::USER_DEFINED_LIST)
        {
            const UserDefinedListLiteral* listLiteral = dynamic_cast<const UserDefinedListLiteral*>(top);

            this->visitedLiterals.push(new WholeNumberLiteral(listLiteral->elements.size()));
        }
        else
        {
            const FunctionDefinedListLiteral* listLiteral = dynamic_cast<const FunctionDefinedListLiteral*>(top);

            if (listLiteral->isInfinite)
            {
                throw std::runtime_error("The length of an infinite list is undefined");
            }

            this->visitedLiterals.push(new WholeNumberLiteral(listLiteral->numberOfElements));
        }
    }
    else if (token->name == "list")
    {
        if (top->isList())
        {
            throw std::runtime_error("Expected a number in the \'list\' function");
        }
        else
        {
            this->visitedLiterals.push(new FunctionDefinedListLiteral(this->getNumber(top), 1, -1));
        }
    }
    else if (token->name == "write")
    {
        std::cout << top->toString() << std::endl;

        this->visitedLiterals.push(new WholeNumberLiteral(0));
    }
    else if (token->name == "int")
    {
        if (top->isList())
        {
            throw std::runtime_error("Expected a number in the \'list\' function");
        }
        else
        {
            double number = this->getNumber(top);

            this->visitedLiterals.push(new WholeNumberLiteral(static_cast<int>(number)));
        }
    }
    else
    {
        throw std::runtime_error("Unknown function name");
    }
    
    delete top;
}

void Interpreter::visitBinaryFunction(const ASTBinaryFunctionNode* node)
{
    const FunctionNameToken* token = dynamic_cast<const FunctionNameToken*>(node->token);

    const Literal* second = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    const Literal* first = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    if (this->userFunctions.find(token->name) != this->userFunctions.end())
    {
        this->userArguments.push_back(this->copy(first));
        this->userArguments.push_back(this->copy(second));

        this->visit(this->userFunctions[token->name]);
    }
    else if (token->name == "eq")
    {
        if (first->isList() && second->isList())
        {
            this->visitedLiterals.push(new WholeNumberLiteral(this->compareLists(first, second)));
        }
        else if (first->isNumber() && second->isNumber())
        {
            double firstValue = this->getNumber(first);
            double secondValue = this->getNumber(second);

            this->visitedLiterals.push(new WholeNumberLiteral(fabs(firstValue - secondValue) < Interpreter::EPS));
        }
        else if (first->isNumber() && second->isList())
        {
            this->visitedLiterals.push(new WholeNumberLiteral(this->compareNumberAndList(first, second)));
        }
        else
        {
            this->visitedLiterals.push(new WholeNumberLiteral(this->compareNumberAndList(second, first)));
        }
    }
    else if (token->name == "le")
    {
        if (first->isList() || second->isList())
        {
            throw std::runtime_error("Expected two numbers in the \'le\' function");
        }

        double firstValue = this->getNumber(first);
        double secondValue = this->getNumber(second);

        this->visitedLiterals.push(new WholeNumberLiteral(firstValue < secondValue));
    }
    else if (token->name == "nand")
    {
        if (first->isList() || second->isList())
        {
            throw std::runtime_error("Expected two numbers in the \'land\' function");
        }

        double firstValue = this->getNumber(first);
        double secondValue = this->getNumber(second);

        this->visitedLiterals.push(new WholeNumberLiteral(!firstValue || !secondValue));
    }
    else if (token->name == "list")
    {
        if (first->isList() || second->isList())
        {
            throw std::runtime_error("Expected two numbers in the \'list\' function");
        }
        else
        {
            this->visitedLiterals.push(new FunctionDefinedListLiteral(this->getNumber(first), this->getNumber(second), -1));
        }
    }
    else if (token->name == "concat")
    {
        if (first->isNumber() || second->isNumber())
        {
            throw std::runtime_error("Expected two list in the \'concat\' function");
        }
        else if (this->isInfiniteList(first))
        {
            throw std::runtime_error("Impossible concatination");
        }
        else if (this->isInfiniteList(second))
        {
            const FunctionDefinedListLiteral* secondFunctionDefined = dynamic_cast<const FunctionDefinedListLiteral*>(second);
            this->visitedLiterals.push(new FunctionDefinedListLiteral(this->getList(first).front(), secondFunctionDefined->step, secondFunctionDefined->numberOfElements));
        }
        else
        {
            std::list<double> firstList = this->getList(first);
            std::list<double> secondList = this->getList(second);

            for (double current : secondList)
            {
                firstList.push_back(current);
            }

            this->visitedLiterals.push(new UserDefinedListLiteral(firstList));
        }
    }
    else if (token->name == "add")
    {
        if (first->isList() || second->isList())
        {
            throw std::runtime_error("Expected two numbers in the \'add\' function");
        }

        double firstValue = this->getNumber(first);
        double secondValue = this->getNumber(second);

        double result = firstValue + secondValue;

        const Literal* toAdd = nullptr;
        if(this->isInt(result))
        {
            toAdd = new WholeNumberLiteral(result);
        } 
        else
        {
            toAdd = new FractionalNumberLiteral(result);
        }

        this->visitedLiterals.push(toAdd);
    }
    else if (token->name == "sub")
    {
        if (first->isList() || second->isList())
        {
            throw std::runtime_error("Expected two numbers in the \'sub\' function");
        }

        double firstValue = this->getNumber(first);
        double secondValue = this->getNumber(second);

        double result = firstValue - secondValue;

        const Literal* toAdd = nullptr;
        if(this->isInt(result))
        {
            toAdd = new WholeNumberLiteral(result);
        } 
        else
        {
            toAdd = new FractionalNumberLiteral(result);
        }

        this->visitedLiterals.push(toAdd);
    }
    else if (token->name == "mul")
    {
        if (first->isList() || second->isList())
        {
            throw std::runtime_error("Expected two numbers in the \'mul\' function");
        }

        double firstValue = this->getNumber(first);
        double secondValue = this->getNumber(second);

        double result = firstValue * secondValue;

        const Literal* toAdd = nullptr;
        if(this->isInt(result))
        {
            toAdd = new WholeNumberLiteral(result);
        } 
        else
        {
            toAdd = new FractionalNumberLiteral(result);
        }

        this->visitedLiterals.push(toAdd);
    }
    else if (token->name == "div")
    {
        if (first->isList() || second->isList())
        {
            throw std::runtime_error("Expected two numbers in the \'div\' function");
        }

        double firstValue = this->getNumber(first);
        double secondValue = this->getNumber(second);

        if (fabs(secondValue - 0) < Interpreter::EPS)
        {
            throw std::runtime_error("Cannot divide by 0");
        }

        double result = firstValue / secondValue;

        const Literal* toAdd = nullptr;
        if(this->isInt(result))
        {
            toAdd = new WholeNumberLiteral(result);
        } 
        else
        {
            toAdd = new FractionalNumberLiteral(result);
        }

        this->visitedLiterals.push(toAdd);
    }
    else if (token->name == "mod")
    {
        if (first->isList() || second->isList())
        {
            throw std::runtime_error("Expected two numbers in the \'mod\' function");
        }

        if (first->type == LiteralType::FRACTIONAL_NUMBER || second->type == LiteralType::FRACTIONAL_NUMBER)
        {
            throw std::runtime_error("Expected two whole numbers in the \'mod\' function");
        }

        int firstValue = dynamic_cast<const WholeNumberLiteral*>(first)->value;
        int secondValue = dynamic_cast<const WholeNumberLiteral*>(second)->value;

        this->visitedLiterals.push(new WholeNumberLiteral(firstValue % secondValue));
    }

    else
    {
        throw std::runtime_error("Unknown function name");
    }

    delete first;
    delete second;
}

void Interpreter::visitIf(const ASTTernaryFunctionNode* node)
{
    this->visit(node->firstArgument);

    const Literal* condition = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    if (condition->isList())
    {
        throw std::runtime_error("Expected a number for the condition in the \'if\' function");
    }
    else
    {
        bool isTrue = this->getNumber(condition);

        isTrue ? this->visit(node->secondArgument) : this->visit(node->thirdArgument);
    }

    delete condition;
}

void Interpreter::visitTernaryFunction(const ASTTernaryFunctionNode* node)
{
    const FunctionNameToken* token = dynamic_cast<const FunctionNameToken*>(node->token);

    const Literal* third = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    const Literal* second = this->visitedLiterals.top();
    this->visitedLiterals.pop();
    const Literal* first = this->visitedLiterals.top();
    this->visitedLiterals.pop();

    if (this->userFunctions.find(token->name) != this->userFunctions.end())
    {
        this->userArguments.push_back(this->copy(first));
        this->userArguments.push_back(this->copy(second));
        this->userArguments.push_back(this->copy(third));

        this->visit(this->userFunctions[token->name]);
    }
    else if (token->name == "list")
    {
        if (first->isList() || second->isList() || third->isList())
        {
            throw std::runtime_error("Expected three numbers in the \'list\' function");
        }
        else
        {
            double firstElement = this->getNumber(first);
            double step = this->getNumber(second);
            double numberOfElements = this->getNumber(third);

            if (!this->isInt(numberOfElements))
            {
                throw std::runtime_error("The third argument of the \'list\' function should be a whole number");
            }

            this->visitedLiterals.push(new FunctionDefinedListLiteral(firstElement, step, numberOfElements));
        }
    }
    else
    {
        throw std::runtime_error("Unknown function name");
    }

    delete first;
    delete second;
    delete third;
}

void Interpreter::visitUserFunction(const ASTUserFunctionNode* node)
{
    const FunctionNameToken* token = dynamic_cast<const FunctionNameToken*>(node->token);

    this->addToUserFunctions(token->name, node->definition);

    this->visitedLiterals.push(new WholeNumberLiteral(0));
}

void Interpreter::visitArgument(const ASTNode* node)
{
    const ArgumentToken* token = dynamic_cast<const ArgumentToken*>(node->token);

    if (token->index > this->userArguments.size())
    {
        std::runtime_error("Too few arguments");
    }

    const Literal* toCopy = this->userArguments[token->index];

    this->visitedLiterals.push(this->copy(toCopy)); 
}

void Interpreter::visit(const ASTNode* node)
{
    if (!node->token)
    {
        this->visitList(dynamic_cast<const ASTListNode*>(node));
    }
    else if (node->token->type == TokenType::WHOLE_NUMBER || 
             node->token->type == TokenType::FRACTIONAL_NUMBER)
    {
        this->visitNumber(node);
    }
    else if (node->token->type == TokenType::ARGUMENT)
    {
        this->visitArgument(node);
    }
    else if (node->token->type == TokenType::FUNCTION_NAME)
    {
        const FunctionNameToken* functionNameToken = dynamic_cast<const FunctionNameToken*>(node->token);

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
        else if (typeid(*node) == typeid(ASTBinaryFunctionNode))
        {
            const ASTBinaryFunctionNode* binaryFunctionNode = dynamic_cast<const ASTBinaryFunctionNode*>(node);
            const ASTNode* firstArgument = binaryFunctionNode->firstArgument;
            const ASTNode* secondArgument = binaryFunctionNode->secondArgument;

            this->visit(firstArgument);
            this->visit(secondArgument);

            this->visitBinaryFunction(binaryFunctionNode);
        }
        else if (typeid(*node) == typeid(ASTTernaryFunctionNode))
        {
            const ASTTernaryFunctionNode* ternaryFunctionNode = dynamic_cast<const ASTTernaryFunctionNode*>(node);

            if (dynamic_cast<const FunctionNameToken*>(ternaryFunctionNode->token)->name == "if")
            {
                this->visitIf(ternaryFunctionNode);
            }
            else
            {
                const ASTNode* firstArgument = ternaryFunctionNode->firstArgument;
                const ASTNode* secondArgument = ternaryFunctionNode->secondArgument;
                const ASTNode* thirdArgument = ternaryFunctionNode->thirdArgument;

                this->visit(firstArgument);
                this->visit(secondArgument);
                this->visit(thirdArgument);

                this->visitTernaryFunction(ternaryFunctionNode);
            }
        }
        else if (typeid(*node) == typeid(ASTUserFunctionNode))
        {
            const ASTUserFunctionNode* userFunctionNode = dynamic_cast<const ASTUserFunctionNode*>(node);

            this->visitUserFunction(userFunctionNode);
        }
    }
}

std::string Interpreter::interpret(const ASTNode* astTree, std::ostream& out)
{
    std::string result; 

    this->deallocate();

    this->visit(astTree);

    if (this->visitedLiterals.empty())
    {
        throw std::runtime_error("Runtime error");
    }
    if (!this->visitedLiterals.empty())
    {
        const Literal* top = this->visitedLiterals.top();

        if (top->type == LiteralType::FUNCTION_DEFINED_LIST)
        {
            const FunctionDefinedListLiteral* listLiteral = dynamic_cast<const FunctionDefinedListLiteral*>(top);

            if (listLiteral->isInfinite)
            {
                double current = listLiteral->firstElement;
                out << "[";
                while (true)
                {
                    out << current << " ";

                    current += listLiteral->step;
                }
            }
        }
        result = top->toString();

        this->visitedLiterals.pop();
        delete top;
    }

    if (!this->visitedLiterals.empty())
    {
        throw std::runtime_error("Too many arguments");
    }

    return result;
}