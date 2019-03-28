#include <iostream>
#include <cstdlib>
#include "includes/error.hpp"
using namespace std;

void throw_error(int error_code, size_t & p)
{
    cout << "Their was a error parsing your code at " << p << endl;
    exit(error_code);
}
