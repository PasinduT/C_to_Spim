#include "includes/parser.hpp"
#include "includes/error.hpp"
#include "includes/token.hpp"
#include <iostream>

Program * parse(vector<Token> & tokens, size_t & p)
{
    for (int i = 0; i < tokens.size(); i++)
    {
        cout << i << " - " << tokens[p].str_val;
        tokens[i].print();
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

TOKENS peek(vector<Token> & tokens, size_t & p)
{
    if (p == tokens.size())
    {
        return TOK_NOTHING;
    }
    return tokens[p+1].token_type;
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

Condition * parse_condition(vector<Token> & tokens, size_t & p)
{
    Condition * result = new Condition;
    result->first = parse_r_value(tokens, p);

    inc_pointer(tokens, p);
    if (tokens[p].token_type == TOK_L_ANGLE)
    {
        result->type = LESS;
    }
    else if (tokens[p].token_type == TOK_R_ANGLE)
    {
        result->type = GREATER;
    }
    else if (tokens[p].token_type == TOK_EQUAL && 
                ((p < (tokens.size() - 1)) && tokens[p+1].token_type == TOK_EQUAL))
    {
        inc_pointer(tokens, p);
        result->type = EQUAL;
    }
    else
    {
        throw_error(0, p);
    }
    inc_pointer(tokens, p);

    result->second = parse_r_value(tokens, p);
    return result;
}

Term * parse_term(vector<Token> & tokens, size_t & p)
{
    Term * result = new Term;
    result->first = parse_factor(tokens, p);
    if (p < tokens.size() - 1)
    {
        if (tokens[p + 1].token_type == TOK_ASTERISK)
        {
            result->type = MULTIPLY;
            inc_pointer(tokens, p);
            inc_pointer(tokens, p);
            result->second = parse_factor(tokens, p);
        }
        else if (tokens[p + 1].token_type == TOK_FORWARD_SLASH)
        {
            result->type = DIVIDE;
            inc_pointer(tokens, p);
            inc_pointer(tokens, p);
            result->second = parse_factor(tokens, p);
        }
    }
    return result;
}

R_Value * parse_r_value(vector<Token> & tokens, size_t & p)
{
    R_Value * result = new R_Value;
    result->first = parse_term(tokens, p);
    result->type = NONE;
    R_Value * current = result;
    while (peek(tokens, p) == TOK_PLUS || peek(tokens, p) == TOK_MINUS)
    {
        inc_pointer(tokens, p);
        Binary_Operator_Type bin_op = PLUS;
        if (tokens[p].token_type == TOK_MINUS)
        {
            bin_op = MINUS;
        }
        
        inc_pointer(tokens, p);
        if (current->type == NONE)
        {
            current->type = bin_op;
            current->second = parse_term(tokens, p);
        }
        else
        {
            result = new R_Value;
            result->type = MULTIPLE_MINUS;
            if (bin_op == PLUS)
            {
                result->type = MULTIPLE_PLUS;
            }
            result->left = current;
            result->second = parse_term(tokens, p);
            current = result;
        }
    }
    return result;
}

Factor * parse_factor(vector<Token> & tokens, size_t & p)
{
    Factor * result = new Factor;
    if (tokens[p].token_type == TOK_INT)
    {
        result->int_val = tokens[p].int_val;
        result->type = INT_VAL;
        return result;
    }

    string identifier;
    if (tokens[p].token_type != TOK_IDENTIFIER)
    {
        throw_error(0, p);
    }
    identifier = tokens[p].str_val;
    result->identifier = identifier;
    result->type = IDENTIFIER;
    if ((p < tokens.size() - 1) && tokens[p+1].token_type == TOK_L_BRACKET)
    {
        inc_pointer(tokens, p);
        inc_pointer(tokens, p);
        result->type = ARRAY_IDENTIFIER;
        result->array_index = parse_r_value(tokens, p);
        
        inc_pointer(tokens, p);
        if (tokens[p].token_type != TOK_R_BRACKET)
        {
            throw_error(0, p);
        }
    }
    return result;
}



Printf_Statement * parse_printf_statement(vector<Token> & tokens, size_t & p)
{   
    if (tokens[p].token_type != TOK_PRINTF)
    {
        throw_error(0, p);
    }

    inc_pointer(tokens, p);
    if (tokens[p].token_type != TOK_L_PARAN)
    {
        throw_error(0, p);
    }

    inc_pointer(tokens, p);
    string str_val;
    if (tokens[p].token_type != TOK_STRING)
    {
        throw_error(0, p);
    }
    str_val = tokens[p].str_val;

    inc_pointer(tokens, p);
    if (tokens[p].token_type != TOK_R_PARAN)
    {
        throw_error(0, p);
    }
    Printf_Statement * result = new Printf_Statement;
    (*result).str_val = str_val;
    return result;
}

Declare_Statement * parse_declare_statement(vector<Token> & tokens, size_t & p)
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

    Declare_Statement * result = new Declare_Statement;
    result->identifier = identifier;
    if ((p < tokens.size() - 1) && (tokens[p+1].token_type == TOK_L_BRACKET))
    {
        result->is_array = true;
        inc_pointer(tokens, p);
        inc_pointer(tokens, p);

        int int_val;
        if (tokens[p].token_type != TOK_INT)
        {
            throw_error(0, p);
        }
        int_val = tokens[p].int_val;
        inc_pointer(tokens, p);
        if (tokens[p].token_type != TOK_R_BRACKET)
        {
            throw_error(0, p);
        }

        result->int_val = int_val;
    }

    return result;
}

Assignment_Statement * parse_assignment_statement(vector<Token> & tokens, size_t & p)
{
    Assignment_Statement * result = new Assignment_Statement;

    string identifier;
    if (tokens[p].token_type != TOK_IDENTIFIER)
    {
        throw_error(0, p);
    }
    identifier = tokens[p].str_val;

    inc_pointer(tokens, p);
    if (tokens[p].token_type == TOK_L_BRACKET)
    {
        inc_pointer(tokens, p);
        result->array_index = parse_r_value(tokens, p);
        result->is_array = true;

        inc_pointer(tokens, p);
        if (tokens[p].token_type != TOK_R_BRACKET)
        {
            throw_error(0, p);
        }
        inc_pointer(tokens, p);
    }
    if (tokens[p].token_type != TOK_EQUAL)
    {
        throw_error(0, p);
    }

    inc_pointer(tokens, p);
    result->r_value = parse_r_value(tokens, p);
    result->identifier = identifier;
    return result;
}

Statement * parse_statement(vector<Token> & tokens, size_t & p)
{
    Statement * result;
    if (tokens[p].token_type == TOK_RETURN)
    {
        Return_Statement * rstmt = parse_return_statement(tokens, p);
        result = rstmt;
        result->type = RETURN;
    }
    else if (tokens[p].token_type == TOK_PRINTF)
    {
        Printf_Statement * pstmt = parse_printf_statement(tokens, p);
        result = pstmt;
        result->type = PRINTF;
    }
    else if (tokens[p].token_type == TOK_IF)
    {
        If_Statement * istmt = parse_if_statement(tokens, p);
        result = istmt;
        result->type = IF;
        return result;
    }
    else if (tokens[p].token_type == TOK_INT_TYPE)
    {
        Declare_Statement * dstmt = parse_declare_statement(tokens, p);
        result = dstmt;
        result->type = DECLARE;
    }
    else if (tokens[p].token_type == TOK_IDENTIFIER)
    {
        if ((p < tokens.size() - 1) && ((tokens[p+1].token_type == TOK_EQUAL)
                || (tokens[p+1].token_type == TOK_L_BRACKET)))
        {
            Assignment_Statement * astmt = parse_assignment_statement(tokens, p);
            result = astmt;
            result->type = ASSIGNMENT;
        }
    }

    inc_pointer(tokens, p);
    if (tokens[p].token_type != TOK_SEMICOLON)
    {
        throw_error(5, p);
    }
    return result;
}

Statement * parse_multi_statemnt(vector<Token> & tokens, size_t & p)
{
    Statement * first = parse_statement(tokens, p);
    inc_pointer(tokens, p);
    if (tokens[p].token_type == TOK_R_BRACE)
    {
        return first;
    }
    Statement * result = new Statement;
    result->type = MULTI;
    result->first = first;
    result->second = parse_statement(tokens, p);

    Statement * current = result;
    
    inc_pointer(tokens, p);
    while (tokens[p].token_type != TOK_R_BRACE)
    {
        Statement * temp = new Statement;
        temp->type = MULTI;
        temp->first = current->second;
        temp->second = parse_statement(tokens, p);
        current->second = temp;
        current = temp;
        inc_pointer(tokens, p);
    }
    return result;
}

If_Statement * parse_if_statement(vector<Token> & tokens, size_t & p)
{
    If_Statement * istmt = new If_Statement;
    istmt->type = IF;
    if (tokens[p].token_type != TOK_IF)
    {
        throw_error(0, p);
    }

    inc_pointer(tokens, p);
    if (tokens[p].token_type != TOK_L_PARAN)
    {
        throw_error(0, p);
    }

    inc_pointer(tokens, p);
    istmt->condition = parse_condition(tokens, p);

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
    istmt->body = parse_multi_statemnt(tokens, p);

    if (tokens[p].token_type != TOK_R_BRACE)
    {
        throw_error(0, p);
    }

    if ((p < tokens.size() - 1) && tokens[p + 1].token_type == TOK_ELSE)
    {
        istmt->has_else = true;
        inc_pointer(tokens, p);
        inc_pointer(tokens, p);
        if (tokens[p].token_type == TOK_IF)
        {
            istmt->else_body = parse_if_statement(tokens, p);
        }
        else
        {
            if (tokens[p].token_type != TOK_L_BRACE)
            {
                throw_error(0, p);
            }
            inc_pointer(tokens, p);
            istmt->else_body = parse_multi_statemnt(tokens, p);
            if (tokens[p].token_type != TOK_R_BRACE)
            {
                throw_error(0, p);
            }
        }
    }
    return istmt;
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
    Statement * stmt = parse_multi_statemnt(tokens, p);

    Function * result = new Function;
    (*result).stmt = stmt;
    (*result).identifier = identifier;
    return result;
}