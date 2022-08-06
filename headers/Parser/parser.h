/** @file parser.h
 *  @brief A class for constructing an Abstract Syntax Tree (AST) using the tokens from the Lexer
 *  
 *  @author Peter Kolev
 */

#pragma once

#include <vector>
#include "token.h"
#include "ASTNode.h"

/**
 * @class Parser
 */
class Parser
{
private:
    std::string text; //!< stores the input
    std::vector<Token*> tokens; //!< stores the tokens generated from the lexer
    std::vector<Token*>::iterator it; //!< current position

    //! Helper function for generating a number node - whole or fractional
    ASTNode* number();

    //! Helper function for generating a list node
    ASTNode* list();

    //! Helper function for generating a function node - unary, binary, ternary, etc.
    ASTNode* function();

    void deallocateNodes(std::vector<const ASTNode*>& nodes);

public:

    //! Basic constructor with parameters
    Parser(const std::string& text, const std::vector<Token*>& tokens);
    
    //! Deleted copy constructor
    Parser(const Parser& other) = delete;

    //! Deleted copy assignment operator
    Parser operator = (const Parser& other) = delete;

    //! Default destructor
    ~Parser() = default;

    //! Main function which generates the AST
    ASTNode* parse();
};