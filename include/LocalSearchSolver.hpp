/**
 * Implementation of a MaxSAT solver based on local search
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef LOCALSEARCHSOLVER_H
#define LOCALSEARCHSOLVER_H

#include <time.h>
#include "../include/MaxSATSolver.hpp"

/**
 * @brief A MaxSAT solver based on local search metaheuristic
 */
class LocalSearchSolver : public MaxSATSolver {
  public:
    using MaxSATSolver::MaxSATSolver;
    LocalSearchSolver(const SATInstance &instance, uint seed = time(NULL));

    virtual void solve();

    int eval_function(vector<bool> &assignment, int flipped_var);
    void do_local_search(int penalty_sum);
    void print_solution();

    uint seed;

    // Maps a variable to the clauses it appears in
    vector<vector<int>> affected_clauses;
};

#endif