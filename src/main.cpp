#include <iostream>
#include <string>
#include <fstream>
#include "includes/CLI11.hpp"
#include "includes/parser.hpp"
#include "includes/lex.hpp"
#include "includes/code_gen.hpp"
using namespace std;

int main(int argc, char * argv[])
{
    // Set up the command line argument parser
    CLI::App app{"C to SPIM compiler"};
    string input_file;
    app.add_option("-i", input_file, "The name of the input file")->required();
    string output_file = "cout";
    app.add_option("-o", output_file, "The name of the output file");
    CLI11_PARSE(app, argc, argv);

    ifstream input (input_file);
    if (!input.is_open())
    {
        cerr << "The file \"" << input_file << "\" could not be opened" << endl;
        return 1;
    }
    vector<Token> tokens;
    lex(input, tokens);
    size_t p = 0;
    Program * program = parse(tokens, p); 
    if (output_file == "cout")
        gen_output(program, cout);
    else 
    {
        ofstream output;
        output.open(output_file);
        if (output.fail())
        {
            cerr << "The file \"" << output_file << "\" could not be opened" << endl;
            return 1;
        }
        gen_output(program, output);
        output.close();
    }
    return 0;
}
