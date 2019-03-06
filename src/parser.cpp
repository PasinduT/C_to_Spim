#include "includes/parser.hpp"

void parse(vector<Token> & tokens)
{
    for (auto t : tokens)
    {
        t.print();
    }
}