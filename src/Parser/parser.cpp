#include "parser.h"

Parser::Parser(const std::vector<Token*>& tokens)
{
    this->tokens = tokens;
    this->currentIndex = 0;
}

ASTLeafNode* Parser::parse()
{
    
}

Parser::~Parser()
{
    for (Token* current : this->tokens)
    {
        delete current;
    }
}