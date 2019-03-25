#include "includes/parser.hpp"
#include "includes/error.hpp"
#include "includes/token.hpp"

Program * parse(vector<Token> & tokens, size_t & p)
{
    for (auto t : tokens)
    {
        t.print();
    }

    Function * func = parse_function(tokens, p);
    
    Program * result = new Program;
    (*result).func = func;
    return result;
}

void inc_pointer(vector<Token> & tokens, size_t & p)
{
    p++;
    if (p >= tokens.size())
    {
        throw_error(1, p);
    }
}

Return_Statement * parse_return_statement(vector<Token> & tokens, size_t & p)
{
    if (tokens[p].token_type != TOK_RETURN)
    {
        throw_error(0, p);
    }

    inc_pointer(tokens, p);

    int int_val;
    if (tokens[p].token_type != TOK_INT)
    {
        throw_error(0, p);
    }
    int_val = tokens[p].int_val;

    Return_Statement * result = new Return_Statement;
    (*result).int_val = int_val;
    return result;
}

Statement * parse_statement(vector<Token> & tokens, size_t & p)
{
    Return_Statement * rtstmt = parse_return_statement(tokens, p);
    inc_pointer(tokens, p);
    if (tokens[p].token_type != TOK_SEMICOLON)
    {
        throw_error(0, p);
    }

    Statement * result = new Statement;
    result->rstmt = rtstmt;
    return result;
}

Function * parse_function(vector<Token> & tokens, size_t & p)
{
    if (tokens[p].token_type != TOK_INT_TYPE)
    {
        throw_error(0, p);
    }

    inc_pointer(tokens, p);

    string identifier;
    if (tokens[p].token_type != TOK_IDENTIFIER)
    {
        throw_error(0, p);
    }
    identifier = tokens[p].str_val;

    inc_pointer(tokens, p);
    if (tokens[p].token_type != TOK_L_PARAN)
    {
        throw_error(0, p);
    }

    inc_pointer(tokens, p);
    if (tokens[p].token_type != TOK_R_PARAN)
    {
        throw_error(0, p);
    }

    inc_pointer(tokens, p);
    if (tokens[p].token_type != TOK_L_BRACE)
    {
        throw_error(0, p);
    }

    inc_pointer(tokens, p);
    Statement * stmt = parse_statement(tokens, p);

    inc_pointer(tokens, p);
    if (tokens[p].token_type != TOK_R_BRACE)
    {
        throw_error(0, p);
    }

    Function * result = new Function;
    (*result).stmt = stmt;
    (*result).identifier = identifier;
    return result;
}