/**
 * Implementation of a Max-SAT solver based on ant colony optimization (ACO)
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <iostream>
#include <cmath>

#include "../include/SATInstance.hpp"
#include "../include/ACOSolver.hpp"

/**
 * @brief 
 * 
 * @param instance The SAT instance
 * @param seed The seed for the random number generator
 * @return ACOSolver:: 
 */
ACOSolver::ACOSolver(
    const SATInstance &instance,
    int n_ants,

    double alpha,
    double beta,
    double rho,
    double q0,
    double tau0,
    
    uint seed
) : MaxSATSolver(instance),
    seed(seed),
    n_ants(n_ants),
    alpha(alpha),
    beta(beta),
    rho(rho),
    q0(q0),
    tau0(tau0)
{
    // Initialize the population with random solutions
    srand(seed);

    // Initialize the pheromone vector 
    for (int i = 0; i < instance.n_vars; i++) {
        pheromone.push_back(tau0);
    }

    // Initialize the heuristic vector
    for (int i = 0; i < instance.n_vars; i++) {
        heuristic.push_back(1);
    }

    // Compute the probability denominator
    for (int i = 0; i < instance.n_clauses; i++)
        sum_probs += pow(pheromone[i], alpha) * pow(heuristic[i], beta);
}

/**
 * @brief Solves the SAT instance using the ACO algorithm
 */
void ACOSolver::solve() {
    // For each iteration
    for (int i = 0; i < MAX_SEARCHES; i++) {
        // For each ant
        for (int j = 0; j < n_ants; j++) {
            // Generate a random solution
            vector<bool> assignment = generate_solution();

            // Compute the number of satisfied clauses
            int n_satisfied = compute_n_satisfied(assignment);

            // If the solution is better than the current best, update the best
            if (n_satisfied > optimal_n_satisfied) {
                optimal_n_satisfied = n_satisfied;
                optimal_assignment = assignment;
            }

            optimal_found = instance.n_clauses == optimal_n_satisfied;
            if (optimal_found) return;
        }

        // Blur the pheromone vector
        for (int j = 0; j < instance.n_vars; j++) {
            pheromone[j] *= (1 - rho);
        }
        sum_probs *= (1 - rho);

        // For each variable in the best solution
        for (int j = 0; j < instance.n_vars; j++) {
            // If the variable is true in the best solution
            if (optimal_assignment[j]) {
                // Increase the pheromone of the variable
                // Other options: pheromone[j] += q0 / optimal_n_satisfied;
                //                pheromone[j] += q0
                pheromone[j] += q0;
                sum_probs += q0;
            }
        }
    }
}

vector<bool> ACOSolver::generate_solution() {
    // Initialize the solution vector
    vector<bool> assignment(instance.n_vars, false);

    // For each variable
    for (int i = 0; i < instance.n_vars ; i++) {
        // Compute the probability of setting the variable to true
        double p = (
            pow(pheromone[i], alpha) * pow(heuristic[i], beta) / sum_probs
        );

        // Set the variable to true with probability p
        bool value = ((double) rand() / RAND_MAX) < p;
        assignment[i] = value;
    }

    return assignment;
}

void ACOSolver::print_solution() {
    cout << "c seed = " << seed << endl;
    MaxSATSolver::print_solution();
}