/**
 * Implementation of a MaxSAT solver with brute force
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */
#include <iostream>
#include <vector>

#include "../include/SATInstance.hpp"
#include "../include/BruteForceSolver.hpp"

using namespace std;

/**
 * @brief Generates an initial solution for the instance to be solved
 * 
 * @param instance The SAT instance
 */
BruteForceSolver::BruteForceSolver(const SATInstance &instance) : MaxSATSolver(instance) {
    optimal_assignment = vector<bool>(instance.n_vars, false);
}

/**
 * Implementation of a MaxSAT solver with brute force
 */
void BruteForceSolver::solve() {
    // Initialize the assignment
    vector<bool> assignment = optimal_assignment;

    // Try every assignment
    solve_helper(assignment, 0);

    // Optimum is guaranteed to be found
    optimal_found = true;
}

/**
 * @brief Recursively generates all the possible assignments and computes the
 * n_satisfied_clauses of each one, stops when the optimal assignment is found
 * 
 * @param assignment 
 * @param i 
 */
void BruteForceSolver::solve_helper(vector<bool> &assignment, int i) {
    // If every clause is already satisfied, stop
    if (optimal_found) return;

    // If all the variables have been assigned
    if (i == instance.n_vars) {
        // Compute the n_satisfied_clauses of the assignment
        int n_satisfied_clauses = compute_n_satisfied(assignment);

        // Update the optimal assignment
        if (n_satisfied_clauses > optimal_n_satisfied) {
            optimal_n_satisfied = n_satisfied_clauses;
            optimal_assignment = assignment;
        }

        optimal_found = instance.n_clauses == optimal_n_satisfied;
        return;
    }

    // Try assigning the variable to false
    for (bool boolean : {false, true}) {
        assignment[i] = boolean;
        solve_helper(assignment, i+1);
    }
}