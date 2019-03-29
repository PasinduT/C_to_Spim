#include "includes/lex.hpp"
#include <cctype>
using namespace std;

void lex(ifstream & input, vector<Token> & tokens)
{
    vector<int> registers(32, 30);
    char c;
    input.get(c);
    while (input)
    {
        if (c == ' ' || c == '\n')
        {
            input.get(c);
            continue;
        }
        if (c == '#')
        {
            // Its an include or define statement
            // Ignore for now (by reading till end of line)
            while (input.get(c) && c != '\n') ;
            input.get(c);
        }
        else if (c == '"')
        {
            Token str_token;
            str_token.token_type = TOK_STRING;
            while (input.get(c) && c != '"') 
            {
                str_token.str_val += c;
            }
            input.get(c);
            tokens.push_back(str_token);
        }
        else if (isalpha(c))
        {
            string iden;
            iden += c;

            while (input.get(c) && isalpha(c))
            {
                iden += c;
            }

            Token token;
            if (iden == "printf")
            {
                token.token_type = TOK_PRINTF;
            }
            else if (iden == "return")
            {
                token.token_type = TOK_RETURN;
            }
            else if (iden == "int")
            {
                token.token_type = TOK_INT_TYPE;
            }
            else if (iden == "if")
            {
                token.token_type = TOK_IF;
            }
            else
            {
                token.token_type = TOK_IDENTIFIER;
                token.str_val = iden;
            }
            tokens.push_back(token);
        }
        else if (isdigit(c))
        {
            Token token;
            token.str_val += c;

            while (input.get(c) && isdigit(c))
            {
                token.str_val += c;
            }
            token.int_val = stoi(token.str_val);
            token.token_type = TOK_INT;
            tokens.push_back(token);
        }
        else 
        {
            Token token;
            switch (c)
            {
                case ';':  token.token_type = TOK_SEMICOLON;
                            break;

                case '{':  token.token_type = TOK_L_BRACE;
                            break;

                case '}':  token.token_type = TOK_R_BRACE;
                            break;

                case '[':  token.token_type = TOK_L_BRACKET;
                            break;

                case ']':  token.token_type = TOK_R_BRACKET;
                            break;

                case '(':  token.token_type = TOK_L_PARAN;
                            break;

                case ')':  token.token_type = TOK_R_PARAN;
                            break;

                case '=':  token.token_type = TOK_EQUAL;
                            break;
                
                case '<':  token.token_type = TOK_L_ANGLE;
                            break;

                case '>':  token.token_type = TOK_R_ANGLE;
                            break;
            }
            tokens.push_back(token);
            input.get(c);
        }
    }
}