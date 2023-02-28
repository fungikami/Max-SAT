/**
 * Implementation of a Max-SAT solver based on guided local search (GLS)
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <iostream>
#include <vector>

#include "../include/GLSSolver.hpp"
#include "../include/SATInstance.hpp"

using namespace std;

/**
 * @brief Generates an initial solution for the instance to be solved, using
 * the given seed
 * 
 * @param instance The SAT instance
 * @param seed The seed for the random number generator
 */
GLSSolver::GLSSolver(const SATInstance &instance, uint seed)
    : MaxSATSolver(instance),
    seed(seed),
    affected_clauses(instance.n_vars, vector<int>()),
    penalty(instance.n_clauses, 0)
{
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
void GLSSolver::solve() {
    // Compute the initial number of satisfied clauses
    int internal_optimal_n_satisfied;
    vector<bool> internal_optimal_assignment = optimal_assignment;

    while (trials < MAX_TRIALS) {
        // Local search algorithm
        int i = 0;
        int current_n_satisfied = compute_n_satisfied(internal_optimal_assignment);
        internal_optimal_n_satisfied = current_n_satisfied - (
            param * penalty_sum(internal_optimal_assignment)
        );

        while (i < instance.n_vars) {
            vector<bool> assignment = internal_optimal_assignment;

            for (i = 0; i < instance.n_vars; i++) {
                // Flip a variable and evaluate the new assignment
                assignment[i] = !assignment[i];

                pair<int, int> new_n_satisfied = evaluate_guided_flip(
                    assignment, i, current_n_satisfied
                );
                if (new_n_satisfied.first > internal_optimal_n_satisfied) {
                    internal_optimal_n_satisfied = new_n_satisfied.first;
                    internal_optimal_assignment = assignment;
                    current_n_satisfied = new_n_satisfied.second;
                    break;
                }

                // Undo the flip
                assignment[i] = !assignment[i];
            }

            optimal_found = instance.n_clauses == internal_optimal_n_satisfied;
            if (optimal_found) break;
        }

        // Update the overall optimal assignment if the internal optimal is better
        if (internal_optimal_n_satisfied > optimal_n_satisfied) {
            optimal_assignment = internal_optimal_assignment;
            optimal_n_satisfied = current_n_satisfied;
        }

        if (optimal_found) break;

        // Calculate the utility of each clause
        priority_queue<pair<double, int>> utility;
        for (int i = 0; i < instance.n_clauses; i++) {
            bool i_s = indicator(internal_optimal_assignment, i);
            double util = i_s / (double) (1 + penalty[i]);
            utility.push(make_pair(util, i));
        }

        // For each clause with maximum utility, augment the penalty
        pair<double, int> max_utility = utility.top();
        do {
            int index = utility.top().second;
            penalty[index]++;
            utility.pop();
        } while (utility.size() && utility.top().first == max_utility.first);

        trials++;
    }
}

/**
 * @brief Evaluates the given assignment
 * 
 * @param assignment The assignment to be evaluated
 * @param flipped_var The variable that was flipped to obtain the assignment
 * @param current_n_satisfied The number of satisfied clauses of the current assignment
 * @return int The new number of satisfied clauses of the assignment
 */
pair<int, int> GLSSolver::evaluate_guided_flip(
    vector<bool> &assignment,
    int flipped_var,
    int current_n_satisfied
) {
    int new_n_satisfied = current_n_satisfied;

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

    return make_pair(
        new_n_satisfied - param * penalty_sum(assignment),
        new_n_satisfied
    );
}

/**
 * @brief Indicate if the clause i is violated by the assignment
 *        true if violated | false otherwise
 * 
 * @param assignment The assignment to be evaluated
 * @param i The clause to be evaluated
 */
bool GLSSolver::indicator(vector<bool> &assignment, int i) {
    // Scans every literal in the clause to see if it is violated
    for (int literal : instance.clauses[i])
        if (instance.is_literal_true(literal, assignment))
            return false;

    return true;
}

int GLSSolver::penalty_sum(vector<bool> &assignment) {
    // Compute the new number of satisfied_clauses, skipping the first iteration
    int sum = 0;
    if (trials != 0) {
        for (int i = 0; i < instance.n_clauses; i++) {
            bool i_s = indicator(assignment, i);
            sum += i_s * penalty[i];
        }
    }

    return sum;
}

void GLSSolver::print_solution() {
    cout << "c seed = " << seed << endl;
    MaxSATSolver::print_solution();
}