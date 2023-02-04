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
 * Implementation of a MaxSAT solver with brute force
 */
void BruteForceSolver::solve() {
    // Initialize the assignment
    vector<bool> assignment(instance.n_vars, false);

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
        int weight = 0;

        // For each clause
        int n_satisfied = 0;
        for (int j = 0; j < instance.n_clauses; j++) {
            bool satisfied = false;

            // For each variable in the clause
            for (auto var : instance.clauses[j]) {
                bool var_assignment = assignment[var>>1];

                // var & 1 is true if var is negated
                satisfied = var & 1 ? !var_assignment : var_assignment;

                // If the clause is satisfied, skip to the next one
                if (satisfied) {
                    weight += instance.weights[j];
                    n_satisfied++;
                    break;
                }
            }
        }

        // Update the optimal assignment
        if (weight > max_weight) {
            max_weight = weight;
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