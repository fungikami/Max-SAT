/**
 * Implementation of a MaxSAT solver based on local search
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef SOLUTIONTREESOLVER_H
#define SOLUTIONTREESOLVER_H

#define MAX_GROWS 1000

#include <time.h>

#include "../include/MaxSATSolver.hpp"

/**
 * @brief A MaxSAT solver based on local search metaheuristic
 */
class SolutionTreeSolver : public MaxSATSolver {
  public:
    using MaxSATSolver::MaxSATSolver;
    SolutionTreeSolver(
      const SATInstance &instance,
      int branching_factor,
      int max_depth,
      uint seed = time(NULL)
    );

    int iteration = 0;
    int branching_factor;
    int max_depth;

    virtual void solve();

    int eval_function(vector<bool> &assignment, int flipped_var, int current_n_satisfied);
    void print_solution();

    uint seed;
};

#endif