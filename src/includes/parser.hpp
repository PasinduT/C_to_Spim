#pragma once

#include <vector>
#include "token.hpp"

enum Statement_Type
{
    RETURN,
    PRINTF,
    EMPTY,
    MULTI,
    DECLARE,
    ASSIGNMENT,
    IF
};

enum R_Value_Type
{
    INT_VAL,
    IDENTIFIER,
    ARRAY_IDENTIFIER
};

enum Logical_Operator_Type
{
    LESS,
    GREATER,
    EQUAL
};

class R_Value
{
public:
    R_Value_Type type;
    int int_val;
    string identifier;
    R_Value * array_index;
};

class Statement
{
public:
    Statement_Type type;
    Statement * first;
    Statement * second;
    Statement()
    {
        type = EMPTY;
    }

    virtual ~Statement() {}
};

class Condition 
{
public:
    R_Value * first;
    R_Value * second;
    Logical_Operator_Type type;
};

class Return_Statement : public Statement
{
public:
    int int_val;
};

class Assignment_Statement : public Statement
{
public:
    string identifier;
    R_Value * array_index;
    bool is_array;
    R_Value * r_value;

    Assignment_Statement() {is_array = false;}
};

class Printf_Statement : public Statement
{
public:
    string str_val;
};

class Declare_Statement : public Statement
{
public:
    string identifier;
    bool is_array;
    int int_val;

    Declare_Statement() { is_array = false; }
};

class If_Statement : public Statement
{
public:
    bool has_else = false;
    Condition * condition;
    Statement * body;
    Statement * else_body;
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

R_Value * parse_r_value(vector<Token> & tokens, size_t & p);

Function * parse_function(vector<Token> & tokens, size_t & p);

Condition * parse_condition(vector<Token> & tokens, size_t & p);

Statement * parse_statement(vector<Token> & tokens, size_t & p);

If_Statement * parse_if_statement(vector<Token> & tokens, size_t & p);

Return_Statement * parse_return_statement(vector<Token> & tokens, size_t & p);

Printf_Statement * parse_printf_statement(vector<Token> & tokens, size_t & p);

Declare_Statement * parse_declare_statement(vector<Token> & tokens, size_t & p);

Assignment_Statement * parse_assignment_statement(vector<Token> & tokens, size_t & p);
