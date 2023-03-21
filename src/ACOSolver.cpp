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

    tau0 = instance.n_vars / (1 - rho);
    q0 = 0;

    // Initialize the pheromone vector
    int n_vertices = 2*instance.n_vars - 2;
    for (int i = 0; i < n_vertices; i++) {
        pheromone.push_back(make_pair(tau0, tau0));
    }
    src_pheromone = make_pair(tau0, tau0);

    // Initialize the heuristic vector
    for (int i = 0; i < n_vertices; i++) {
        heuristic.push_back(make_pair(1.0, 1.0));
    }
    src_heuristic = make_pair(1.0, 1.0);
}

/**
 * @brief Solves the SAT instance using the ACO algorithm
 */
void ACOSolver::solve() {
    // For each iteration
    for (int i = 0; i < MAX_SEARCHES; i++) {
        int internal_optimal_n_satisfied = 0;
        vector<bool> internal_optimal_assignment;

        // For each ant
        for (int j = 0; j < n_ants; j++) {
            // Generate a random solution
            vector<bool> assignment = generate_solution();

            // Compute the number of satisfied clauses
            int n_satisfied = compute_n_satisfied(assignment);

            // If the solution is better than the current best, update the best
            if (n_satisfied > internal_optimal_n_satisfied) {
                internal_optimal_n_satisfied = n_satisfied;
                internal_optimal_assignment = assignment;
            }

            optimal_found = instance.n_clauses == internal_optimal_n_satisfied;
            if (optimal_found) break;
        }

        // If the solution is better than the current best, update the best
        if (internal_optimal_n_satisfied > optimal_n_satisfied) {
            optimal_n_satisfied = internal_optimal_n_satisfied;
            optimal_assignment = internal_optimal_assignment;
        }

        // If the solution is optimal, stop the search
        optimal_found = instance.n_clauses == optimal_n_satisfied;
        if (optimal_found) break;

        // Evaporate the pheromones
        src_pheromone.first *= (1 - rho);
        src_pheromone.second *= (1 - rho);
        for (uint j = 0; j < pheromone.size(); j++) {
            pheromone[j].first *= (1 - rho);
            pheromone[j].second *= (1 - rho);
        }

        bool current_src = internal_optimal_assignment[0];
        int literal = current_src;
        if (current_src) src_pheromone.second += q0 * internal_optimal_n_satisfied;
        else src_pheromone.first += q0 * internal_optimal_n_satisfied;

        // For each variable in the optimal assignment
        for (int k = 1; k < instance.n_vars; k++) {
            if (literal > 2*instance.n_vars - 3) break;

            // Saves a reference to the current pheromone
            pair<double, double> &ref = pheromone[literal];

            // Calculate the next node
            literal += 1 + !current_src;
            current_src = internal_optimal_assignment[k];
            literal += current_src;

            // If the literal is true in the best solution
            if (current_src) ref.second += q0 * internal_optimal_n_satisfied;
            else ref.first += q0 * internal_optimal_n_satisfied;
        }

        // Print pheronomes like a list in python
        // cout << "Pheromones: [";
        // for (uint i = 0; i < pheromone.size(); i++) {
        //     int var = (i & 1) ? (i >> 1) : -(i >> 1);
        //     cout << "var " << var << ": ";
        //     cout << "(" << pheromone[i].first << ", " << pheromone[i].second << ")";
        //     if (i != pheromone.size() - 1) cout << ", ";
        // }
        // cout << "]" << endl;
    }
}

vector<bool> ACOSolver::generate_solution() {
    // Initialize the solution vector
    vector<bool> assignment;

    // Calculate probability src_node 
    double p;
    p = pow(src_pheromone.first, alpha) * pow(src_heuristic.first, beta);
    p /= p + pow(src_pheromone.second, alpha) * pow(src_heuristic.second, beta);

    bool current_src = rand() < p * RAND_MAX;
    int literal = current_src;
    // Walk the graph
    while (true) {
        assignment.push_back(current_src);
        // End of the graph
        if (literal > 2*instance.n_vars - 3) break;

        // Calculate the next node according to the probability
        p = pow(pheromone[literal].first, alpha) * pow(heuristic[literal].first, beta);
        p /= p + pow(pheromone[literal].second, alpha) * pow(heuristic[literal].second, beta);

        // Calculate the next node
        literal += 1 + !current_src;
        current_src = rand() < p * RAND_MAX;
        literal += current_src;
    }

    return assignment;
}

void ACOSolver::print_solution() {
    cout << "c seed = " << seed << endl;
    MaxSATSolver::print_solution();
}