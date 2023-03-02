/**
 * Implementation of a Max-SAT solver based on simulated annealing
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef SIMULATEDANNEALINGSOLVER_H
#define SIMULATEDANNEALINGSOLVER_H

#define MAX_ITER 100
#define COOLING_FACTOR 0.99

#include <time.h>
#include <vector>

#include "../include/MaxSATSolver.hpp"

/**
 * @brief A MaxSAT solver based on genetic algorithm
 */
class SimulatedAnnealingSolver : public MaxSATSolver {
  public:
    using MaxSATSolver::MaxSATSolver;

    SimulatedAnnealingSolver(
        const SATInstance &instance,
        int max_stagnation,
        int max_no_improvement,
        double initial_temperature,
        uint seed = time(NULL)
    );

    virtual void solve();

    // void print_solution();

    uint seed;
    int max_iterations = MAX_ITER;
    int max_stagnation;
    int max_no_improvement;
    double initial_temperature;
    double cooling_factor = COOLING_FACTOR;

    // vector<bool> solution;
    // int fitness;

    // void perturb(vector<bool> &solution);
    // double acceptance_probability(int delta, double temperature);
};

#endif
