#include "includes/code_gen.hpp"
#include "includes/error.hpp"
#include <sstream>
#include <iostream>
#include <unordered_map>
using namespace std;

typedef stringstream sstream;
unsigned string_prompts = 0;
unsigned s_registers = 0;
unsigned exits = 0;
unsigned elses = 0;
unordered_map<string, pair<int, string> > symbol_table;

void gen_stmt_code(Statement * stmt, sstream & out, sstream & data);
void gen_code(Program * program, sstream & code, sstream & data);
void gen_code_assignment(Assignment_Statement * astmt, sstream & out, sstream & data);
void gen_code_condition(Condition cond, sstream & out, sstream & data, string target);
void gen_data(sstream & out, string name, string & data);
void gen_data(sstream & out, string name, int data);
string get_var_location(sstream & out, string name, string target);
void gen_r_value(sstream & out, R_Value * r_value, string target);
void gen_term(sstream & out, Term * term, string target);
void gen_factor(sstream & out, Factor * factor, string target);
void gen_data_array(sstream & out, string name, int n);
void gen_output(Program * program, ostream & out);

/**
 * Get the value of r_value into a register. 
 * It destroys the 
 * - $t0 register if the target is not the $t0 register
 * - $t8 register otherwise
 */
void gen_r_value(sstream & out, R_Value * r_value, string target)
{
	string other = "$t0";
	if (r_value->type == NONE)
	{
		gen_term(out, r_value->first, target);
		return;
	}
	if (other == target)
	{
		other = "$t8";
		if (r_value->type == PLUS)
		{
			gen_term(out, r_value->second, other);
			gen_term(out, r_value->first, target);
			out << "\t\tadd " << target << ", " << target << ", " << other << endl;
			out << endl;
		}
		else if (r_value->type == MINUS)
		{
			gen_term(out, r_value->second, other);
			gen_term(out, r_value->first, target);
			out << "\t\tsub " << target << ", " << target << ", " << other << endl;
			out << endl;
		}
		else if (r_value->type == MULTIPLE_PLUS)
		{
			gen_r_value(out, r_value->left, other);
			gen_term(out, r_value->second, target);
			out << "\t\tadd " << target << ", " << target << ", " << other << endl;
			out << endl;
		}
		else if (r_value->type == MULTIPLE_MINUS)
		{
			gen_r_value(out, r_value->left, other);
			gen_term(out, r_value->second, target);
			out << "\t\tsub " << target << ", " << target << ", " << other << endl;
			out << endl;
		}
	}
	else 
	{
		if (r_value->type == PLUS)
		{
			gen_term(out, r_value->first, target);
			gen_term(out, r_value->second, other);
			out << "\t\tadd " << target << ", " << target << ", " << other << endl;
			out << endl;
		}
		else if (r_value->type == MINUS)
		{
			gen_term(out, r_value->first, target);
			gen_term(out, r_value->second, other);
			out << "\t\tsub " << target << ", " << target << ", " << other << endl;
			out << endl;
		}
		else if (r_value->type == MULTIPLE_PLUS)
		{
			gen_r_value(out, r_value->left, other);
			gen_term(out, r_value->second, target);
			out << "\t\tadd " << target << ", " << target << ", " << other << endl;
			out << endl;
		}
		else if (r_value->type == MULTIPLE_MINUS)
		{
			gen_r_value(out, r_value->left, other);
			gen_term(out, r_value->second, target);
			out << "\t\tsub " << target << ", " << target << ", " << other << endl;
			out << endl;
		}
	}
}

/**
 * Get the value of r_value into a register. 
 * It destroys the 
 * - $t0 register if the target is not the $t0 register
 * - $t8 register otherwise
 */
void gen_term(sstream & out, Term * term, string target)
{
	if (term->type == NONE)
	{
		gen_factor(out, term->first, target);
		return;
	}
	/* TODO: Implement the multiplication and division parts
	string other = "$t0";
	if (target == other)
	{
		other = "$t8";
		gen_factor(out, term->first, other);
		gen_factor(out, term->second, other);
	}
	*/
}

/**
 * Get the value of factor into a register. 
 * If it is an array it destroys the 
 * - $t0 register if the target is not the $t0 register
 * - $t9 register otherwise
 */
void gen_factor(sstream & out, Factor * factor, string target)
{
	if (factor->type == INT_VAL)
	{
		out << "\t\tli "<< target << ", " << factor->int_val << endl;
		return;
	}

	if (symbol_table.find(factor->identifier) == symbol_table.end())
	{
		cout << "ID: " << factor->identifier << endl;
		cout.flush();
		size_t some = 0;
		throw_error(23, some);
	}

	if (factor->type == IDENTIFIER)
	{
		string location = get_var_location(out, factor->identifier, target);
		if (location.at(0) == '$')
		{
			if (location != target)
				out << "\t\tmove " << target << ", " << location << endl;
		}
		else
		{
			out << "\t\tlw " << target << ", " << "0(" << target << ")" << endl;
		}
	}
	if (factor->type == ARRAY_IDENTIFIER)
	{
		string other = "$t0";
		if (other == target)
		{
			other = "$t9";
		}
		gen_r_value(out, factor->array_index, other);
		out << "\t\tsll "<< other << ", " << other << ", 2" << endl;
		string location = get_var_location(out, factor->identifier, target);
		out << "\t\tadd " << target << ", " << other << ", " << location << endl;
		out << "\t\tlw " << target << ", 0(" << target << ")" << endl;
	}
}

void gen_code_condition(Condition * cond, sstream & out, sstream & data, string target)
{
	if (cond->type == LESS)
	{
		gen_r_value(out, cond->first, "$t1");
		gen_r_value(out, cond->second, "$t0");
		out << "\t\tslt " << target << ", " << "$t1" << ", " << "$t0" << endl;
	}
	else if (cond->type == GREATER)
	{
		gen_r_value(out, cond->first, "$t1");
		gen_r_value(out, cond->second, "$t0");
		out << "\t\tslt " << target << ", " << "$t0" << ", " << "$t1" << endl;
	}
	else if (cond->type == EQUAL)
	{
		gen_r_value(out, cond->first, "$t1");
		gen_r_value(out, cond->second, "$t0");
		out << "\t\tbne " << "$t0" << ", " << "$t1, " << target << endl;
	}
}

void gen_code_assignment(Assignment_Statement * astmt, sstream & out, sstream & data)
{
	if (symbol_table.find(astmt->identifier) == symbol_table.end())
	{
		size_t some = 0;
		throw_error(3, some);
	}
	string location = symbol_table[astmt->identifier].second;
	if (astmt->is_array == false)
	{
		if (location.at(0) == '$')
		{
			gen_r_value(out, astmt->r_value, location);
			out << endl;
		}
		else 
		{
			gen_r_value(out, astmt->r_value, "$t1");
			out << "\t\tla $t0, " << location << endl;
			out << "\t\tsw $t1, 0($t0)" << endl << endl;
		}
	}
	else
	{
		// Array on the left hand side
		if (location.at(0) == '$')
		{
			gen_r_value(out, astmt->r_value, "$t1");
			gen_r_value(out, astmt->array_index, "$t0");

			out << "\t\tsll $t0, $t0, 2" << endl;
			out << "\t\tadd $t0, $t0, " << location << endl;
			out << "\t\tsw $t1, 0($t0)" << endl << endl;
		}
		else 
		{
			gen_r_value(out, astmt->r_value, "$t1");
			gen_r_value(out, astmt->array_index, "$t0");
			
			out << "\t\tla $t2, " << location << endl;
			out << "\t\tsll $t0, $t0, 2" << endl;
			out << "\t\tadd $t0, $t0, $t2" << endl;
			out << "\t\tsw $t1, 0($t0)" << endl << endl;
		}
	}
}

void gen_stmt_code(Statement * stmt, sstream & out, sstream & data)
{
    if (stmt->type == RETURN)
    {
		Return_Statement * rstmt = dynamic_cast<Return_Statement *>(stmt);
        out << "\t\tli $v0, " << rstmt->int_val << endl;
        delete rstmt;
    }
    else if (stmt->type == PRINTF)
    {
		Printf_Statement * pstmt = dynamic_cast<Printf_Statement *>(stmt);
        string prompt = "prompt" + to_string(string_prompts++);
        gen_data(data, prompt, 
                pstmt->str_val);
        out << "\t\tla $a0, " << prompt << endl;
        out << "\t\tli $v0, 4" << endl;
        out << "\t\tsyscall" << endl << endl;
    }
	else if (stmt->type == IF)
    {
		If_Statement * istmt = dynamic_cast<If_Statement *>(stmt);
		string exit_point = "EXIT" + to_string(exits++);
		string branch_point = (istmt->has_else ? "ELSE" + to_string(elses++) : exit_point);
		if (istmt->condition->type == EQUAL)
		{
			gen_code_condition(istmt->condition, out, data, branch_point);
		}
		else 
		{
			gen_code_condition(istmt->condition, out, data, "$t1");
			out << "\t\tbeq " << "$t1, " << "$zero, " << branch_point << endl << endl;
		}
		gen_stmt_code(istmt->body, out, data);
		if (istmt->has_else)
		{
			out << "\t\tj " << exit_point << endl;
			out << branch_point << ":" << endl;
			gen_stmt_code(istmt->else_body, out, data);
		}
		out << exit_point << ":" << endl;
    }
    else if (stmt->type == MULTI)
    {
        gen_stmt_code(stmt->first, out, data);
        gen_stmt_code(stmt->second, out, data);
    }
    else if (stmt->type == DECLARE)
    {
		Declare_Statement * dstmt = dynamic_cast<Declare_Statement *>(stmt);
        if (symbol_table.find(dstmt->identifier) != symbol_table.end())
        {
            size_t some = 0;
            throw_error(2, some);
        }
		if (dstmt->is_array == false)
		{
			if (s_registers <= 7)
			{
				symbol_table[dstmt->identifier] = make_pair(0, 
							"$s" + to_string(s_registers++));
			}
			else 
			{
				string identifier = dstmt->identifier;
				gen_data(data, identifier, 0);
				symbol_table[dstmt->identifier] = make_pair(0, 
							identifier);
			}
	 	}
		else
		{
			gen_data_array(data, dstmt->identifier, dstmt->int_val);
			if (s_registers <= 7)
			{
				out << "\t\tla $s" << s_registers << ", " << 
					dstmt->identifier << endl << endl;
				symbol_table[dstmt->identifier] = make_pair(1, 
							"$s" + to_string(s_registers++));
			}
			else 
			{
				symbol_table[dstmt->identifier] = make_pair(1, 
							dstmt->identifier);
			}
		}
		
    }
    else if (stmt->type == ASSIGNMENT)
    {
		Assignment_Statement * astmt = dynamic_cast<Assignment_Statement *>(stmt);
        gen_code_assignment(astmt, out, data);
    }
}
/**
 * This function gives the value of a location as a register
 */
string get_var_location(sstream & out, string name, string target)
{
	if (symbol_table.find(name) == symbol_table.end())
	{
		size_t some = 0;
		throw_error(7, some);
	}
	name = symbol_table[name].second;
	if (name.at(0) != '$')
	{
		out << "\t\tla " << target << ", " << name << endl;
		// out << "\t\tlw " << target << ", 0(" << target << ")" << endl;
		return name;
	}
	return name;
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

void gen_data_array(sstream & out, string name, int n)
{
	out << "\t\t.align 2" << endl; 
   out << name << ":\t.space " << n << endl;
}

void gen_output(Program * program, ostream & out)
{
    sstream data, code;
    data << "\t\t.data" << endl;
    code << "\t\t.text" << endl;
    gen_code(program, code, data);
    out << data.str() << endl << code.str() << endl;
}