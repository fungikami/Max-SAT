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
    // Try every assignment
    solve_helper(0);

    // Optimal is guaranteed to be found
    optimal_found = true;
}

void BruteForceSolver::solve_helper(int i) {
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

        if (n_satisfied == instance.n_clauses) optimal_found = true;
        return;
    }

    // For each possible assignment of the variables compute the assignment
    for (int j = 0; j <= 1; j++) {
        assignment[i] = j;
        solve_helper(i+1);
    }
}