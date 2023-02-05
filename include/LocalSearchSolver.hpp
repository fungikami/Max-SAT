/**
 * Implementation of a MaxSAT solver based on local search
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef LOCALSEARCHSOLVER_H
#define LOCALSEARCHSOLVER_H

#include "../include/SATSolver.hpp"

/**
 * @brief A MaxSAT solver based on local search metaheuristic
 */
class LocalSearchSolver : public SATSolver {
  public:
    using SATSolver::SATSolver;
    LocalSearchSolver(const SATInstance &instance);
    LocalSearchSolver(const SATInstance &instance, time_t seed);

    virtual void solve();

    time_t seed;

    // Maps a variable to the clauses it appears in
    vector<vector<int>> affected_clauses;

    int evaluate_flip(vector<bool> &assignment, int flipped_var, int &new_satisfied);
    void print_solution();
};

#endif