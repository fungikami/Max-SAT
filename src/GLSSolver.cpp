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
    // Compute the initial weight
    optimal_weight = compute_weight(optimal_assignment);

    // Local search
    int trials = 0;
    while (trials < 100) {
        int i = 0;
        // ============== No existe penalty ==============
        while (i < instance.n_vars) {
            vector<bool> assignment = optimal_assignment;

            for (i = 0; i < instance.n_vars; i++) {
                // Flip a variable
                assignment[i] = !assignment[i];

                // Prints the penalty vector as a Python list

                int new_weight = evaluate_guided_flip(assignment, i);
                if (new_weight > optimal_weight) {

                    // =========== DEBUG ===========
                    // if (trials > 0) {
                        // cout << "c     weight = " << optimal_weight << endl;
                        // cout << "c new weight = " << new_weight << endl;
                    // }
                    // =========== DEBUG ===========

                    optimal_weight = new_weight;
                    optimal_assignment = assignment;

                    break;
                }

                // Undo the flip
                assignment[i] = !assignment[i];
            }

            optimal_found = instance.total_weight == optimal_weight;
            if (optimal_found) break;
        }
        // ============== Hasta aqui ==============

        // Calculate the utility of each clause
        priority_queue<pair<double, int>> utility;
        for (int i = 0; i < instance.n_clauses; i++) {
            if (indicator(optimal_assignment, i)) {
                double util = instance.weights[i] / (double) (1 + penalty[i]);
                utility.push(make_pair(util, i));
            } else 
                utility.push(make_pair(0, i));
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
 * @return int The new weight of the assignment
 */
int GLSSolver::evaluate_guided_flip(
    vector<bool> &assignment,
    int flipped_var
) {
    int new_weight = optimal_weight;

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
                new_weight += instance.weights[i];
            else
                new_weight -= instance.weights[i];
        }
    }

    // Compute the new weight
    int sum = 0;

    for (int i = 0; i < instance.n_clauses; i++)  {
        if (indicator(optimal_assignment, i)) sum += penalty[i];
        // if (penalty[i]) cout << "c penalty[" << i << "] = " << penalty[i] << endl;
    }

    return new_weight - param * sum;
}

/**
 * @brief Indicate if the clause i is violated by the assignment
 *        true if violated | false otherwise
 */
bool GLSSolver::indicator(vector<bool> &assignment, int i) {
    // Scans every literal in the clause to see if it is violated
    for (int literal : instance.clauses[i])
        if (instance.is_literal_true(literal, assignment))
            return false;

    return true;
}

void GLSSolver::print_solution() {
    cout << "c seed = " << seed << endl;
    MaxSATSolver::print_solution();
}