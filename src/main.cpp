/**
 * Solver de Max-SAT
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <functional>
#include <iostream>
#include <chrono>

#include "../include/BruteForceSolver.hpp"
#include "../include/SATInstance.hpp"

using namespace std;

double measure_time(function<void()> f);

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
    // - See if we can factorize the weight computation into another function,
    // and maybe make use of the watchlist for that purpose
    SATInstance instance(argv[1]);

    // Initialize every solver
    BruteForceSolver solver(instance);

    // Benchmark every solver
    double brute_force_t = measure_time([&] { solver.solve(); });

    // For debugging purposes
    solver.print_solution();

    // Verify the solution is valid
    // solver.verify_solution();

    // Print the time elapsed
    cout << "Time elapsed:" << endl;
    cout << "  Exact solver: " << brute_force_t << "s" << endl;

    return 0;
}

/**
 * @brief Function to measure the time elapsed executing a function
 * 
 * @param f The function to be executed
 * @return double The time elapsed in seconds
 */
double measure_time(function<void()> f) {
    auto start = chrono::high_resolution_clock::now();
    f();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::duration<double>>(end - start).count();
}