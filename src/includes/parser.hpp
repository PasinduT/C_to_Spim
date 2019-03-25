#pragma once

#include <vector>
#include "token.hpp"

class Return_Statement 
{
public:
    int int_val;
};

class Statement
{
public:
    int type;
    Return_Statement * rstmt;
};

class Function 
{
public:
    string identifier;
    Statement * stmt;
};

class Program
{
public:
    Function * func;
};



/**
 * Parses an entire program
 * @param tokens The list of tokens
 * @param p the position of the initial token 
 */
Program * parse(vector<Token> & tokens, size_t & p);

Function * parse_function(vector<Token> & tokens, size_t & p);

Statement * parse_statement(vector<Token> & tokens, size_t & p);

Return_Statement * parse_return_statement(vector<Token> & tokens, size_t & p);
