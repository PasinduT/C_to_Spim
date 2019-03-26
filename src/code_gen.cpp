#include "includes/code_gen.hpp"
#include <sstream>
#include <iostream>
using namespace std;

typedef stringstream sstream;
unsigned string_prompts = 0;

void gen_stmt_code(Statement * stmt, sstream & out, sstream & data);
void gen_code(Program * program, sstream & code, sstream & data);
void gen_data(sstream & out, string name, string & data);
void gen_output(Program * program, ostream & out);

void gen_stmt_code(Statement * stmt, sstream & out, sstream & data)
{
    if (stmt->type == RETURN)
    {
        out << "\t\tli $v0, " << stmt->rstmt->int_val << endl;
    }
    else if (stmt->type == PRINTF)
    {
        string prompt = "prompt" + to_string(string_prompts++);
        gen_data(data, prompt, 
                stmt->pstmt->str_val);
        out << "\t\tla $a0, " << prompt << endl;
        out << "\t\tli $v0, 4" << endl;
        out << "\t\tsyscall" << endl;
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

void gen_output(Program * program, ostream & out)
{
    sstream data, code;
    data << "\t\t.data" << endl;
    code << "\t\t.text" << endl;
    gen_code(program, code, data);
    out << data.str() << endl << code.str() << endl;
}