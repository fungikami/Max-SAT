/**
 * Implementation of a Max-SAT solver based on simulated annealing
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

// #include <iostream>

// #include "../include/SATInstance.hpp"
// #include "../include/SimulatedAnnealingSolver.hpp"

// SimulatedAnnealingSolver::SimulatedAnnealingSolver(
//     const SATInstance &instance,
//     int max_stagnation,
//     int max_no_improvement,
//     double initial_temperature,
//     uint seed
// ) : MaxSATSolver(instance),
//     max_stagnation(max_stagnation),
//     max_no_improvement(max_no_improvement),
//     initial_temperature(initial_temperature),
//     seed(seed)
// {
//     // Initialize the optimal assignment with random values
//     srand(seed);
//     for (int i = 0; i < instance.n_vars; i++)
//         optimal_assignment.push_back(rand() & 1);
// }

#include <iostream>

#include "../include/SATInstance.hpp"
#include "../include/SimulatedAnnealingSolver.hpp"

/**
 * @brief Generates an initial solution for the instance to be solved, using
 * the given seed
 * 
 * @param instance The SAT instance
 * @param seed The seed for the random number generator
 */
SimulatedAnnealingSolver::SimulatedAnnealingSolver(
    const SATInstance &instance,
    int max_stagnation,
    int max_no_improvement,
    double initial_temperature,
    uint seed
) : MaxSATSolver(instance),
    seed(seed),
    max_stagnation(max_stagnation),
    max_no_improvement(max_no_improvement),
    initial_temperature(initial_temperature)
{
    // Initialize the optimal assignment with random values
    srand(seed);
    for (int i = 0; i < instance.n_vars; i++)
        optimal_assignment.push_back(rand() & 1);
}

void SimulatedAnnealingSolver::solve() {
    cout << "Solving instance " << instance.n_vars << " " << instance.n_clauses << endl;
}