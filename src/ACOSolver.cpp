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
 * @param n_ants The number of ants
 * @param alpha The alpha parameter
 * @param beta The beta parameter
 * @param rho The rho parameter
 * @param q0 The q0 parameter
 * @param tau0 The tau0 parameter
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
    double max_pheromone = instance.n_vars / (1.0 - rho);
    for (int i = 0; i < instance.n_vars; i++) {
        pheromone.push_back(max_pheromone);
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

        // For each variable in the best solution
        for (int j = 0; j < instance.n_vars; j++) {
            // If the variable is true in the best solution
            if (optimal_assignment[j]) {
                sum_probs -= pheromone[j];

                pheromone[j] *= (1 - rho);
                pheromone[j] += optimal_n_satisfied;

                sum_probs += pheromone[j];
            }
        }

        if (i > 0 && i % 3 == 0) {
            // Blur the pheromone vector
            for (int j = 0; j < instance.n_vars; j++) {
                // Random number between -max and max, where max = 0.9e^(-i/50)
                double max = 0.9 * exp(-i / 50.0);
                double r = (double) rand() / RAND_MAX;
                r = (2*r - 1) * max;

                double value = r * pheromone[j];
                pheromone[j] += value;
                sum_probs += value;
            }
        }

        cout << "i = " << i << ", sum_probs = " << sum_probs << endl;

        // // Prints the pheromone vector as a Python list
        // cout << "pheromone = [";
        // for (int j = 0; j < instance.n_vars; j++) {
        //     cout << pheromone[j];
        //     if (j < instance.n_vars - 1) cout << ", ";
        // }
        // cout << "]" << endl;


    }
}

vector<bool> ACOSolver::generate_solution() {
    // Initialize the solution vector
    vector<bool> assignment(instance.n_vars, false);

    // For each variable
    for (int i = 0; i < instance.n_vars ; i++) {
        // Compute the probability of setting the variable to true
        double p = (pow(pheromone[i], alpha) * pow(heuristic[i], beta)) / sum_probs;

        cout << "p = " << p << endl;

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