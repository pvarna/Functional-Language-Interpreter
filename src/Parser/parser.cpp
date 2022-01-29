#include <iostream>
#include "parser.h"
#include "illegalSyntaxException.h"
#include "ASTListNode.h"
#include "ASTUnaryFunctionNode.h"
#include "ASTBinaryFunctionNode.h"
#include "ASTTernaryFunctionNode.h"
#include "ASTN_aryFunctionNode.h"
#include "ASTUserFunctionNode.h"

Parser::Parser(const std::string& text, const std::vector<Token*>& tokens)
{
    this->text = text;
    this->tokens = tokens;
    this->currentIndex = 0;
}

ASTNode* Parser::number()
{
    if (this->tokens[this->currentIndex]->type != TokenType::WHOLE_NUMBER && 
        this->tokens[this->currentIndex]->type != TokenType::FRACTIONAL_NUMBER)
    {
        throw IllegalSyntaxException("Expected a number", this->text);
    }

    return new ASTNode(this->tokens[this->currentIndex]);
}

ASTNode* Parser::list()
{
    if (this->tokens[this->currentIndex]->type != TokenType::OPENING_SQUARE_BRACKET)
    {
        throw IllegalSyntaxException("Expected a [", this->text);
    }

    ++this->currentIndex;
    if (this->currentIndex >= this->tokens.size())
    {
        throw IllegalSyntaxException("Expected a ]", this->text);
    }

    std::vector<const ASTNode*> elements;
    while (this->tokens[this->currentIndex]->type != TokenType::CLOSING_SQUARE_BRACKET)
    {
        switch (this->tokens[this->currentIndex]->type)
        {
        case TokenType::WHOLE_NUMBER:
        case TokenType::FRACTIONAL_NUMBER:
            elements.push_back(this->number());
            break;
        
        case TokenType::OPENING_SQUARE_BRACKET:
            elements.push_back(this->list());
            break;

        default:
            throw IllegalSyntaxException("Expected a number or another list", this->text);
        }

        ++this->currentIndex;

        if (this->currentIndex >= this->tokens.size())
        {
            throw IllegalSyntaxException("Expected a ]", this->text);
        }
    }

    return new ASTListNode(nullptr, elements);
}

ASTNode* Parser::function()
{
    if (this->tokens[this->currentIndex]->type != TokenType::FUNCTION_NAME)
    {
        throw IllegalSyntaxException("Expected a function name", this->text);
    }

    Token* functionName = this->tokens[this->currentIndex];

    ++this->currentIndex;
    if (this->currentIndex >= this->tokens.size() || 
        this->tokens[this->currentIndex]->type != TokenType::OPENING_ROUND_BRACKET)
    {
        throw IllegalSyntaxException("Expected a (", this->text);
    }
    
    ++this->currentIndex;
    if (this->currentIndex >= this->tokens.size())
    {
        throw IllegalSyntaxException("Expected a )", this->text);
    }

    std::vector<const ASTNode*> arguments;

    while (this->tokens[this->currentIndex]->type != TokenType::CLOSING_ROUND_BRACKET)
    {
        switch (this->tokens[this->currentIndex]->type)
        {
        case TokenType::WHOLE_NUMBER:
        case TokenType::FRACTIONAL_NUMBER:
            arguments.push_back(this->number());
            break;

        case TokenType::OPENING_SQUARE_BRACKET:
            arguments.push_back(this->list());
            break;

        case TokenType::FUNCTION_NAME:
            arguments.push_back(this->function());
            break;

        case TokenType::ARGUMENT:
            arguments.push_back(new ASTNode(this->tokens[this->currentIndex]));
            break;

        default:
            throw IllegalSyntaxException("Expected a number, list, function argument or another function", this->text);
        }

        ++this->currentIndex;

        if (this->currentIndex >= this->tokens.size() ||
            (this->tokens[this->currentIndex]->type != TokenType::COMMA &&
             this->tokens[this->currentIndex]->type != TokenType::CLOSING_ROUND_BRACKET))
        {
            std::cout << this->currentIndex << std::endl;
            throw IllegalSyntaxException("Expected a comma or )", this->text);
        }

        if (this->tokens[this->currentIndex]->type == TokenType::COMMA)
        {
            ++this->currentIndex;
            if (this->currentIndex >= this->tokens.size() || 
                this->tokens[this->currentIndex]->type == TokenType::CLOSING_ROUND_BRACKET)
            {
                throw IllegalSyntaxException("Expected a number, list, function argument or another function", this->text);
            }
        }
    }

    switch (arguments.size())
    {
    case 0:
        return new ASTNode(functionName);
    case 1:
        return new ASTUnaryFunctionNode(functionName, arguments[0]);
    case 2:
        return new ASTBinaryFunctionNode(functionName, arguments[0], arguments[1]);
    case 3:
        return new ASTTernaryFunctionNode(functionName, arguments[0], arguments[1], arguments[2]);
    default:
        return new ASTN_aryFunctionNode(functionName, arguments);
    }

    return nullptr;
}

ASTNode* Parser::parse()
{
    if (this->tokens.empty())
    {
        return nullptr;
    }
    else if (this->tokens[this->currentIndex]->type == TokenType::WHOLE_NUMBER ||
             this->tokens[this->currentIndex]->type == TokenType::FRACTIONAL_NUMBER)
    {
        return this->number();
    }
    else if (this->tokens[this->currentIndex]->type == TokenType::OPENING_SQUARE_BRACKET)
    {
        return this->list();
    }
    else if (this->tokens[this->currentIndex]->type == TokenType::FUNCTION_NAME)
    {
        if (this->currentIndex + 1 >= this->tokens.size() ||
            (this->tokens[this->currentIndex + 1]->type == TokenType::OPENING_ROUND_BRACKET &&
             this->tokens[this->currentIndex + 1]->type == TokenType::ARROW))
        {
            throw IllegalSyntaxException("Expected ( or ->", this->text);
        }
        else if (this->tokens[this->currentIndex + 1]->type == TokenType::OPENING_ROUND_BRACKET)
        {
            return this->function();
        }
        else
        {
            Token* functionName = this->tokens[this->currentIndex];

            this->currentIndex += 2;
            ASTNode* definition = nullptr;
            switch (this->tokens[this->currentIndex]->type)
            {
            case TokenType::WHOLE_NUMBER:
            case TokenType::FRACTIONAL_NUMBER:
                definition = this->number();
                break;

            case TokenType::OPENING_SQUARE_BRACKET:
                definition = this->list();
                break;

            case TokenType::FUNCTION_NAME:
                definition = this->function();
                break;
            
            default:
                throw IllegalSyntaxException("Expected a number, list or function name", this->text);
            }

            return new ASTUserFunctionNode(functionName, definition, {});
        }
    }

    throw IllegalSyntaxException("Invalid beginning. Expected a number, list or function name", this->text);
}

Parser::~Parser()
{
    for (Token* current : this->tokens)
    {
        delete current;
    }
}