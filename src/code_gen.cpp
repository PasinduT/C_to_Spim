#include "includes/code_gen.hpp"

void gen_code(Program * program, ostream & out)
{
    out << "\t\t.data" << endl;
    out << "\t\t.text" << endl;
    out << program->func->identifier << ":" << endl;
    Return_Statement * rstmt = program->func->stmt->rstmt;
    out << "\t\tli $v0, " << rstmt->int_val << endl;
    out << "\t\tj $ra" << endl; 
}