/**
 * Implementation of a MaxSAT solver with brute force
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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

    // Parse the SAT instance
    SATInstance sat_instance(argv[1]);

    // Print the SAT instance
    cout << sat_instance;

    return 0;
}