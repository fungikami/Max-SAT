/**
 * Implementation of a Max-SAT solver based on ant colony optimization (ACO)
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef ACOSOLVER_H
#define ACOSOLVER_H

#define MAX_SEARCHES 10

#include <time.h>
#include <vector>

#include "../include/MaxSATSolver.hpp"

using namespace std;

/**
 * @brief A MaxSAT solver based on ant colony optimization (ACO)
 */
class ACOSolver : public MaxSATSolver {
    public:
        using MaxSATSolver::MaxSATSolver;

        ACOSolver(
            const SATInstance &instance,
            int n_ants,

            double alpha,
            double beta,
            double rho,
            double q0,
            double tau0,

            uint seed = time(NULL)
        );

        virtual void solve();
        void print_solution();

        uint seed;
        int n_ants;

        double alpha;
        double beta;
        double rho;
        double q0;
        double tau0;

        // Pheromone vector
        vector<double> pheromone;

        // Heuristic vector
        vector<double> heuristic;

        double sum_probs = 0;

        vector<bool> generate_solution();
};

#endif
