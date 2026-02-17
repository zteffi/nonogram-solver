
#include <vector>
#include <iostream>
#include "solver.hpp"


int main(int argc, char** argv)
{

    Solver s{ 5, 5,
       { // columns
        {2,2},{1,1},{2},{1,1},{2,2},
       },
       {// rows
        {5}, {1,1,1}, {}, {1,1},{2,2},
    } };

    s.Solve();

    return 0;
}
