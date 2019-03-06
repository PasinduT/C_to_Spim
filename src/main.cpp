#include <iostream>
#include <string>
#include <fstream>
#include "includes/CLI11.hpp"
#include "includes/parser.hpp"
#include "includes/lex.hpp"
using namespace std;

int main(int argc, char * argv[])
{
    // Set up the command line argument parser
    CLI::App app{"C to SPIM compiler"};
    string input_file;
    app.add_option("-i", input_file, "The name of the input file")->required();
    CLI11_PARSE(app, argc, argv);

    ifstream input (input_file);
    if (! input.is_open())
    {
        cerr << "The file \"" << input_file << "\" could not be opened" << endl;
        return 1;
    }
    vector<Token> tokens;
    lex(input, tokens);
    parse(tokens); 

    return 0;
}
