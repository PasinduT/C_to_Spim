#include <iostream>
#include <string>
#include "includes/CLI11.hpp"
using namespace std;

int main(int argc, char * argv[])
{
    // Set up the command line argument parser
    CLI::App app{"C to SPIM compiler"};
    string input_file;
    app.add_option("-i", input_file, "The name of the input file")->required();
    CLI11_PARSE(app, argc, argv);
}
