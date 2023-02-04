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
    // TODO: Hacer que el solver funcione
    // con solve_helper(int i)
    solve_helper(0);
}

void BruteForceSolver::solve_helper(int i) {
    // If all the variables have been assigned
    if (i == instance.n_vars) {
        // Compute the weight of the assignment
        int weight = 0;

        // For each clause
        int n_satisfied = 0;
        for (int j = 0; j < instance.n_clauses; j++) {
            bool satisfied = false;

            for (auto var : instance.clauses[j]) {
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

    // For each possible assignment of the variables compute the assignment
    for (bool j = 0; j <= 1; j++) {
        assignment[i] = j;
        solve_helper(i+1);
    }
}