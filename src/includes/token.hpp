#pragma once

#include <string>
#include <iostream>
using namespace std;

enum TOKENS
{
    TOK_NOTHING,
    TOK_INT,
    TOK_IDENTIFIER,
    TOK_INT_TYPE,
    TOK_INCLUDE,
    TOK_PRINTF,
    TOK_RETURN,
    TOK_STRING,
    TOK_SEMICOLON,
    TOK_L_BRACE,
    TOK_R_BRACE,
    TOK_L_PARAN,
    TOK_R_PARAN,
    TOK_L_ANGLE,
    TOK_R_ANGLE,
    TOK_DOT,
    TOK_EQUAL,
    TOK_L_BRACKET, 
    TOK_R_BRACKET,
    TOK_IF,
    TOK_ELSE,
    TOK_ASTERISK,
    TOK_FORWARD_SLASH,
    TOK_PLUS,
    TOK_MINUS
};

const string TOKEN_STRINGS[] =
{
    "TOK_NOTHING",
    "TOK_INT",
    "TOK_IDENTIFIER",
    "TOK_INT_TYPE",
    "TOK_INCLUDE",
    "TOK_PRINTF",
    "TOK_RETURN",
    "TOK_STRING",
    "TOK_SEMICOLON",
    "TOK_L_BRACE",
    "TOK_R_BRACE",
    "TOK_L_PARAN",
    "TOK_R_PARAN",
    "TOK_L_ANGLE",
    "TOK_R_ANGLE",
    "TOK_DOT",
    "TOK_EQUAL",
    "TOK_L_BRACKET", 
    "TOK_R_BRACKET",
    "TOK_IF",
    "TOK_ELSE",
    "TOK_ASTERISK",
    "TOK_FORWARD_SLASH",
    "TOK_PLUS",
    "TOK_MINUS"
};


class Token
{
public:
    TOKENS token_type;
    string str_val;
    int int_val;

    Token()
    {
        token_type = TOK_INT;
        str_val = "";
        int_val = 0;
    }

    void print()
    {
        cout << TOKEN_STRINGS[token_type] << endl;
    }
};
