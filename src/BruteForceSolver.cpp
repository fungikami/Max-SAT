/**
 * Implementation of a MaxSAT solver with brute force
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */
#include <iostream>
#include <vector>

#include "../include/BruteForceSolver.hpp"
#include "../include/SATInstance.hpp"

using namespace std;

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
 * weight of each one, stops when the optimal assignment is found
 * 
 * @param assignment 
 * @param i 
 */
void BruteForceSolver::solve_helper(vector<bool> &assignment, int i) {
    // If every clause is already satisfied, stop
    if (optimal_found) return;

    // If all the variables have been assigned
    if (i == instance.n_vars) {
        // Compute the weight of the assignment
        int n_satisfied;
        int weight = compute_weight(assignment, n_satisfied);

        // Update the optimal assignment
        if (weight > optimal_weight) {
            optimal_weight = weight;
            optimal_assignment = assignment;
        }

        optimal_found = n_satisfied == instance.n_clauses;
        return;
    }

    // Try assigning the variable to false
    for (bool boolean : {false, true}) {
        assignment[i] = boolean;
        solve_helper(assignment, i+1);
    }
}