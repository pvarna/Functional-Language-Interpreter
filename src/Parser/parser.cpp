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
    this->it = this->tokens.begin();
}

ASTNode* Parser::number()
{
    if ((*it)->type != TokenType::WHOLE_NUMBER && 
        (*it)->type != TokenType::FRACTIONAL_NUMBER)
    {
        throw IllegalSyntaxException("Expected a number", this->text);
    }

    return new ASTNode(*it);
}

void Parser::deallocateNodes(std::vector<const ASTNode*>& nodes)
{
    for (const ASTNode* current : nodes)
    {
        delete current;
    }

    nodes.clear();
}

ASTNode* Parser::list()
{
    if ((*it)->type != TokenType::OPENING_SQUARE_BRACKET)
    {
        throw IllegalSyntaxException("Expected a [", this->text);
    }

    const Token* openingSquareBracket = *it;

    ++it;
    if (it == this->tokens.end())
    {
        throw IllegalSyntaxException("Expected a ]", this->text);
    }

    std::vector<const ASTNode*> elements;
    while ((*it)->type != TokenType::CLOSING_SQUARE_BRACKET)
    {
        const ASTNode* toAdd = nullptr;
        try
        {
            switch ((*it)->type)
            {
            case TokenType::WHOLE_NUMBER:
            case TokenType::FRACTIONAL_NUMBER:
                toAdd = this->number();
                break;
            
            case TokenType::OPENING_SQUARE_BRACKET:
                toAdd = this->list();
                break;

            default:
                throw IllegalSyntaxException("Expected a number or another list", this->text);
            }
        }
        catch(...)
        {
            deallocateNodes(elements);
            throw;
        }

        if (toAdd)
        {
            elements.push_back(toAdd);
        }

        ++it;
        if (it == this->tokens.end())
        {
            deallocateNodes(elements);
            throw IllegalSyntaxException("Expected a ]", this->text);
        }
    }

    try
    {
        return new ASTListNode(openingSquareBracket, elements);
    }
    catch(...)
    {
        deallocateNodes(elements);
        throw;
    }
}

ASTNode* Parser::function()
{
    if ((*it)->type != TokenType::FUNCTION_NAME)
    {
        throw IllegalSyntaxException("Expected a function name", this->text);
    }

    Token* functionName = *it;

    ++it;
    if (it == this->tokens.end() || (*it)->type != TokenType::OPENING_ROUND_BRACKET)
    {
        throw IllegalSyntaxException("Expected a (", this->text);
    }
    
    ++it;
    if (it == this->tokens.end())
    {
        throw IllegalSyntaxException("Expected a )", this->text);
    }

    std::vector<const ASTNode*> arguments;

    while ((*it)->type != TokenType::CLOSING_ROUND_BRACKET)
    {
        const ASTNode* toAdd = nullptr;

        try
        {
            switch ((*it)->type)
            {
            case TokenType::WHOLE_NUMBER:
            case TokenType::FRACTIONAL_NUMBER:
                toAdd = this->number();
                break;

            case TokenType::OPENING_SQUARE_BRACKET:
                toAdd = this->list();
                break;

            case TokenType::FUNCTION_NAME:
                toAdd = this->function();
                break;

            case TokenType::ARGUMENT:
                toAdd = new ASTNode(*it);
                break;

            default:
                throw IllegalSyntaxException("Expected a number, list, function argument or another function", this->text);
            }
        }
        catch(...)
        {
            deallocateNodes(arguments);
            throw;
        }

        if (toAdd)
        {
            arguments.push_back(toAdd);
        }

        ++it;
        if (it == this->tokens.end() ||
            ((*it)->type != TokenType::COMMA &&
             (*it)->type != TokenType::CLOSING_ROUND_BRACKET))
        {
            deallocateNodes(arguments);
            throw IllegalSyntaxException("Expected a comma or )", this->text);
        }

        if ((*it)->type == TokenType::COMMA)
        {
            ++it;
            if (it == this->tokens.end() || 
                (*it)->type == TokenType::CLOSING_ROUND_BRACKET)
            {
                deallocateNodes(arguments);
                throw IllegalSyntaxException("Expected a number, list, function argument or another function", this->text);
            }
        }
    }

    try 
    {
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
    }
    catch(...)
    {
        deallocateNodes(arguments);
        throw;
    }
    

    return nullptr;
}

ASTNode* Parser::parse()
{
    if (it == this->tokens.end())
    {
        return nullptr;
    }
    else if ((*it)->type == TokenType::WHOLE_NUMBER ||
             (*it)->type == TokenType::FRACTIONAL_NUMBER)
    {
        return this->number();
    }
    else if ((*it)->type == TokenType::OPENING_SQUARE_BRACKET)
    {
        return this->list();
    }
    else if ((*it)->type == TokenType::FUNCTION_NAME)
    {
        ++it;
        if (it == this->tokens.end() || ((*it)->type != TokenType::OPENING_ROUND_BRACKET && 
                                         (*it)->type != TokenType::ARROW))
        {
            throw IllegalSyntaxException("Expected ( or ->", this->text);
        }
        else if ((*it)->type == TokenType::OPENING_ROUND_BRACKET)
        {
            --it;
            return this->function();
        }
        else 
        {
            Token* functionName = *(it-1);

            ++it;
            ASTNode* definition = nullptr;
            switch ((*it)->type)
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

            return new ASTUserFunctionNode(functionName, definition);
        }
    }

    throw IllegalSyntaxException("Invalid beginning. Expected a number, list or function name", this->text);
}