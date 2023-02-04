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
    // For each possible assignment of the variables
    for (int i = 0; i < (1 << instance.n_vars); i++) {
        // Compute the assignment
        for (int j = 0; j < instance.n_vars; j++)
            assignment[j] = (i >> j) & 1;

        // Compute the weight of the assignment
        int weight = 0;

        // For each clause
        int n_satisfied = 0;
        for (int j = 0; j < instance.n_clauses; j++) {
            bool satisfied = false;
            for (uint k = 0; k < instance.clauses[j].size(); k++) {
                int var = instance.clauses[j][k];

                // If the variable is negative
                if (var % 2) {
                    // If it's assigned to true, skip the clause
                    if (assignment[var>>1]) continue;

                    // Otherwise, the clause is satisfied
                    satisfied = true;
                    break;

                // If the variable is positive
                } else {
                    // If it's assigned to true, the clause is satisfied
                    if (assignment[var>>1]) {
                        satisfied = true;
                        break;
                    }
                }
            }
            if (satisfied) {
                weight += instance.weights[j];
                n_satisfied++;
            }
        }

        // Update the optimal assignment
        if (weight > max_weight) {
            max_weight = weight;
            optimal_assignment = assignment;
        }

        // If all the clauses are satisfied, stop
        if (n_satisfied == instance.n_clauses) {
            optimal_found = true;
            break;
        }
    }
}