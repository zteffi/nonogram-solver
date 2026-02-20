
#include <vector>
#include <iostream>
#include <ranges>
#include <numeric>
#include <algorithm>
#include <functional>

#include "solver.hpp"

using uchar = unsigned char;


int main(int argc, char** argv)
{

    Solver s{ 7, 6,
       { // columns
        {4,1},{1},{1,1,1},{1},{4,1},
        {1},{4,1}
       },
       {// rows
        {1,1,1},{7},{1,1,1},{1,1,1,1},{},
        {1,1,1,1}
    } };

    s.Solve();

    return 0;
}
