
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

    Solver s{ 8, 5,
       { // columns
        {1,1},{2},{2},{1,2},{1,1,1},
        {3,1},{4},{2}
       },
       {// rows
        {4},{1,3},{8},{1,1,1},{1,2},
    } };

    s.Solve();

    return 0;
}
