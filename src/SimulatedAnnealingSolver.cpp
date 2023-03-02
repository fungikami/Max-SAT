/**
 * Implementation of a Max-SAT solver based on simulated annealing
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */
#include <iostream>
#include <cmath>

#include "../include/SATInstance.hpp"
#include "../include/SimulatedAnnealingSolver.hpp"

/**
 * @brief Generates an initial solution for the instance to be solved, using
 * the given seed
 * 
 * @param instance The SAT instance
 * @param seed The seed for the random number generator
 */
SimulatedAnnealingSolver::SimulatedAnnealingSolver(
    const SATInstance &instance,
    int max_stagnation,
    int max_no_improvement,
    double initial_temperature,
    uint seed
) : MaxSATSolver(instance),
    seed(seed),
    max_stagnation(max_stagnation),
    max_no_improvement(max_no_improvement),
    initial_temperature(initial_temperature),
    temperature(initial_temperature)
{
    // Initialize the optimal assignment with random values
    srand(seed);
    for (int i = 0; i < instance.n_vars; i++)
        optimal_assignment.push_back(rand() & 1);
}

void SimulatedAnnealingSolver::solve() {
    int stagnation = 0;
    int no_improvement = 0;

    optimal_n_satisfied = compute_n_satisfied(optimal_assignment);
    vector<bool> assignment = optimal_assignment;

    int internal_n_satisfied = optimal_n_satisfied;
    while (
        stagnation < max_stagnation &&
        no_improvement < max_no_improvement &&
        iterations < MAX_ITER
    ) {
        while (temperature > 0.0000001) {
            // cout << "Iteration: " << iterations << endl;
            // cout << "Temperature: " << temperature << endl;

            // Take a random neighbor (flip a random variable)
            int i = rand() % instance.n_vars;
            assignment[i] = !assignment[i];

            int new_n_satisfied = eval_function(assignment, i, internal_n_satisfied);
            bool improved = new_n_satisfied > optimal_n_satisfied;

            // Calculate the probability of accepting the neighbor
            double probability = improved ? 1 : 
                exp((new_n_satisfied - optimal_n_satisfied) / temperature);

            // With probability p keeps the current solution
            if (improved || (double) rand() / RAND_MAX < probability) {
                if (improved) {
                    // cout << "Improved!" << endl;
                    // cout << "New n_satisfied: " << new_n_satisfied << endl;

                    optimal_assignment = assignment;
                    optimal_n_satisfied = new_n_satisfied;
                    no_improvement = -1;
                }

                internal_n_satisfied = new_n_satisfied;
                no_improvement++;
                stagnation = 0;
                break;
            }

            assignment[i] = !assignment[i];
            stagnation++;
        }

        temperature *= cooling_factor;
        iterations++;
    }
}

/**
 * @brief Evaluates the given assignment
 * 
 * @param assignment The assignment to be evaluated
 * @param flipped_var The variable that was flipped to obtain the assignment
 * @return int The new number of satisfied clauses of the assignment
 */
int SimulatedAnnealingSolver::eval_function(
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

void SimulatedAnnealingSolver::print_solution() {
    cout << "c seed = " << seed << endl;
    MaxSATSolver::print_solution();
}