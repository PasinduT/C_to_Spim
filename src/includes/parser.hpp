#pragma once

#include <vector>
#include "token.hpp"

enum Statement_Type
{
    RETURN,
    PRINTF,
    EMPTY,
    MULTI,
    DECLARE
};

class Return_Statement 
{
public:
    int int_val;
};

class Printf_Statement 
{
public:
    string str_val;
};

class Declare_Statement 
{
public:
    string identifier;
};

class Statement
{
public:
    Statement_Type type;
    Return_Statement * rstmt;
    Printf_Statement * pstmt;
    Declare_Statement * dstmt;
    Statement * first;
    Statement * second;

    Statement()
    {
        type = EMPTY;
    }
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

Printf_Statement * parse_printf_statement(vector<Token> & tokens, size_t & p);

Declare_Statement * parse_declare_statement(vector<Token> & tokens, size_t & p);
