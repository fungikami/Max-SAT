/**
 * Implementation of a Max-SAT solver based on local search
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

#include "../include/SATInstance.hpp"
#include "../include/SolutionTreeSolver.hpp"

using namespace std;

/**
 * @brief Generates an initial solution for the instance to be solved, using
 * the given seed
 * 
 * @param instance The SAT instance
 * @param seed The seed for the random number generator
 */
SolutionTreeSolver::SolutionTreeSolver(
    const SATInstance &instance,
    int branching_factor,
    int max_depth,
    uint seed
) : MaxSATSolver(instance),
    branching_factor(branching_factor),
    max_depth(max_depth),
    seed(seed)
{
    // Initialize the optimal assignment with random values
    srand(seed);
    for (int i = 0; i < instance.n_vars; i++)
        optimal_assignment.push_back(rand() & 1);
}

/**
 * @brief Solves the instance using local search, taking as a neighborhood the
 * set of assignments that differ from the current one by flipping a single
 * variable
 */
void SolutionTreeSolver::solve() {
    optimal_n_satisfied = compute_n_satisfied(optimal_assignment);
    bool found_better;

    while (iteration < MAX_GROWS) {
        found_better = false;

        // Creates a queue and push the root of the tree
        queue<pair<vector<bool>, int>> q;
        q.emplace(make_pair(optimal_assignment, optimal_n_satisfied));

        for (int i = 0; i < max_depth; i++) {
            queue<pair<vector<bool>, int>> q_aux;
            while (q.size()) {
                // Generates branching_factor children
                for (int j = 0; j < branching_factor; j++) {
                    // Copies the front of the queue to the back of the auxiliar queue
                    q_aux.push(q.front());
                    pair<vector<bool>, int> &child = q_aux.back();

                    // Flips a random variable
                    int k = rand() % instance.n_vars;
                    child.first[k] = !child.first[k];

                    // Evaluates the new assignment
                    child.second = eval_function(child.first, k, child.second);

                    // Checks if the new assignment is better than the current one
                    if (child.second > optimal_n_satisfied) {
                        optimal_assignment = child.first;
                        optimal_n_satisfied = child.second;
                        found_better = true;
                        continue;
                    }

                    // Rejects the new assignment with probability 0.5
                    if (rand() & 1) q_aux.pop();
                }
                q.pop();
            }

            // Swaps the queues
            q = move(q_aux);
        }
        // If no better assignment was found, stop the search
        // if (!found_better) break;

        iteration++;
    }
}

/**
 * @brief Evaluates the given assignment
 * 
 * @param assignment The assignment to be evaluated
 * @param flipped_var The variable that was flipped to obtain the assignment
 * @param current_n_satisfied The number of satisfied clauses before the flip
 * @return int The new number of satisfied clauses of the assignment
 */
int SolutionTreeSolver::eval_function(
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

    return new_n_satisfied;
}

void SolutionTreeSolver::print_solution() {
    cout << "c Solution Tree Solver" << endl;
    cout << "c MAX_GROWS = " << MAX_GROWS << endl;
    cout << "c branching_factor = " << branching_factor << endl;
    cout << "c max_depth = " << max_depth << endl;
    cout << "c seed = " << seed << endl;
    MaxSATSolver::print_solution();
}