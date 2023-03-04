/**
 * Implementation of a Max-SAT solver based on simulated annealing
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef SIMULATEDANNEALINGSOLVER_H
#define SIMULATEDANNEALINGSOLVER_H

#define MAX_ITER 1000000
#define COOLING_FACTOR 0.01

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
        uint seed = time(NULL)
    );

    virtual void solve();

    void print_solution();

    uint seed;
    int iterations = 0;
    double initial_temperature;
    double temperature;
    double cooling_factor = COOLING_FACTOR;

    int eval_function(
      vector<bool> &assignment, 
      int flipped_var,
      int current_n_satisfied
    );
};

#endif
