/**
 * Solver de Max-SAT
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */
#include <iostream>
#include "SATInstance.hpp"

using namespace std;

/**
 * @brief A MaxSAT solver
 * 
 */
class MaxSATSolver {
  void initialize(); // to accept input and initialize
  void solve();      // to solve and show the result
};

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

    // Parse the SAT instance
    SATInstance satInstance(argv[1]);

    // Print the SAT instance
    cout << satInstance;

    return 0;
}