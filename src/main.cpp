/**
 * Solver de Max-SAT
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */
#include <iostream>

#include "../include/BruteForceSolver.hpp"
#include "../include/SATInstance.hpp"

using namespace std;

/**
 * @brief Initialize the solver
 * 
 * @return int
 */
int main(int argc, char *argv[]) {
    // Check if the user provided a filename
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // TODO:
    // - Check if the file exists inside the instance constructor and do
    // something if it doesn't
    // - Make a verifier for the outputted solution to be valid, maybe a
    // separate executable, a SATSolver method or another class.
    SATInstance instance(argv[1]);
    BruteForceSolver solver(instance);
    solver.solve();
    solver.print_solution();

    return 0;
}