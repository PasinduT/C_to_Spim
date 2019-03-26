#include "includes/code_gen.hpp"
#include "includes/error.hpp"
#include <sstream>
#include <iostream>
#include <unordered_map>
using namespace std;

typedef stringstream sstream;
unsigned string_prompts = 0;
unsigned s_registers = 0;
unordered_map<string, pair<int, string> > symbol_table;

void gen_stmt_code(Statement * stmt, sstream & out, sstream & data);
void gen_code(Program * program, sstream & code, sstream & data);
void gen_data(sstream & out, string name, string & data);
void gen_data(sstream & out, string name, int data);
void gen_output(Program * program, ostream & out);

void gen_stmt_code(Statement * stmt, sstream & out, sstream & data)
{
    if (stmt->type == RETURN)
    {
        out << "\t\tli $v0, " << stmt->rstmt->int_val << endl;
        delete stmt->rstmt;
        delete stmt;
    }
    else if (stmt->type == PRINTF)
    {
        string prompt = "prompt" + to_string(string_prompts++);
        gen_data(data, prompt, 
                stmt->pstmt->str_val);
        out << "\t\tla $a0, " << prompt << endl;
        out << "\t\tli $v0, 4" << endl;
        out << "\t\tsyscall" << endl << endl;

    }
    else if (stmt->type == MULTI)
    {
        gen_stmt_code(stmt->first, out, data);
        gen_stmt_code(stmt->second, out, data);
    }

    else if (stmt->type == DECLARE)
    {
        if (symbol_table.find(stmt->dstmt->identifier) != symbol_table.end())
        {
            size_t some = 0;
            throw_error(2, some);
        }
        if (s_registers <= 7)
        {
            symbol_table[stmt->dstmt->identifier] = make_pair(0, 
                        "$s" + to_string(s_registers++));
        }
        else 
        {
            gen_data(data, stmt->dstmt->identifier, 0);
        }
    }
}
void gen_code(Program * program, sstream & code, sstream & data)
{
    code << program->func->identifier << ":" << endl;
    gen_stmt_code(program->func->stmt, code, data);
    code << "\t\tj $ra" << endl; 
}

void gen_data(sstream & out, string name, string & data)
{
   out << name << ":\t.asciiz \"" << data << "\"" << endl;
}

void gen_data(sstream & out, string name, int data)
{
   out << name << ":\t.word " << data  << endl;
}

void gen_output(Program * program, ostream & out)
{
    sstream data, code;
    data << "\t\t.data" << endl;
    code << "\t\t.text" << endl;
    gen_code(program, code, data);
    out << data.str() << endl << code.str() << endl;
}