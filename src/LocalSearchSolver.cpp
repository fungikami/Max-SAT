/**
 * Implementation of a Max-SAT solver based on local search
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <functional>
#include <iostream>
#include <vector>

#include "../include/LocalSearchSolver.hpp"
#include "../include/SATInstance.hpp"

using namespace std;

/**
 * @brief Generates an initial solution for the instance to be solved, using
 * the given seed
 * 
 * @param instance The SAT instance
 * @param seed The seed for the random number generator
 */
LocalSearchSolver::LocalSearchSolver(const SATInstance &instance, uint seed)
    : MaxSATSolver(instance), seed(seed), affected_clauses(instance.n_vars, vector<int>()) {
    // Initialize the optimal assignment with random values
    srand(seed);
    for (int i = 0; i < instance.n_vars; i++)
        optimal_assignment.push_back(rand() & 1);

    // For each clause, save for each variable the clauses it affects
    for (int i = 0; i < instance.n_clauses; i++)
        for (auto literal : instance.clauses[i])
            affected_clauses[literal>>1].push_back(i);
}

/**
 * @brief Solves the instance using local search, taking as a neighborhood the
 * set of assignments that differ from the current one by flipping a single
 * variable
 */
void LocalSearchSolver::solve() {
    do_local_search(0);
}

/**
 * @brief Solves the instance using local search, taking as a neighborhood the
 * set of assignments that differ from the current one by flipping a single
 * variable
 * 
 * @param penalty_sum The sum of the penalties of the clauses that are already
 * satisfied
 */
void LocalSearchSolver::do_local_search(int penalty_sum) {
    int i = 0;
    int current_n_satisfied = compute_n_satisfied(optimal_assignment);
    optimal_n_satisfied = current_n_satisfied - penalty_sum;

    while (i < instance.n_vars) {
        vector<bool> assignment = optimal_assignment;
        for (i = 0; i < instance.n_vars; i++) {
            // Flip a variable
            assignment[i] = !assignment[i];

            int new_n_satisfied = eval_function(assignment, i);
            if (new_n_satisfied > optimal_n_satisfied) {
                optimal_n_satisfied = new_n_satisfied;
                optimal_assignment = assignment;
                break;
            }

            // Undo the flip
            assignment[i] = !assignment[i];
        }

        optimal_found = instance.n_clauses == optimal_n_satisfied;
        if (optimal_found) break;
    }
}

/**
 * @brief Evaluates the given assignment
 * 
 * @param assignment The assignment to be evaluated
 * @param flipped_var The variable that was flipped to obtain the assignment
 * @return int The new number of satisfied clauses of the assignment
 */
int LocalSearchSolver::eval_function(
    vector<bool> &assignment,
    int flipped_var
) {
    int new_n_satisfied = optimal_n_satisfied;

    // Scan the clauses affected by the flipped variable
    for (auto i : affected_clauses[flipped_var]) {

        bool already_satisfied = false;
        int flipped_literal = -1;

        for (auto literal : instance.clauses[i]) {
            if (literal>>1 == flipped_var) {
                if (flipped_literal == -1) flipped_literal = literal;
                else if (flipped_literal != literal) {
                    // Edge case: the clause contains p v -p
                    already_satisfied = true;
                    break;
                }
                continue;
            }

            // Check if the clause was already satisfied regardless of the flip
            already_satisfied = instance.is_literal_true(literal, assignment);
            if (already_satisfied) break;
        }

        // If the clause was not already satisfied, check how the flip affects
        if (!already_satisfied) {
            if (instance.is_literal_true(flipped_literal, assignment)) 
                new_n_satisfied++;
            else
                new_n_satisfied--;
        }
    }

    return new_n_satisfied;
}

void LocalSearchSolver::print_solution() {
    cout << "c seed = " << seed << endl;
    MaxSATSolver::print_solution();
}