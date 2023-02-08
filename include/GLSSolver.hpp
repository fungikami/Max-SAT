/**
 * Implementation of a Max-SAT solver based on guided local search (GLS)
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef GLSSOLVER_H
#define GLSSOLVER_H

#define MAX_TRIALS 100

#include <time.h>
#include <queue>
#include "../include/MaxSATSolver.hpp"

/**
 * @brief A MaxSAT solver based on local search metaheuristic
 */
class GLSSolver : public MaxSATSolver {
  public:
    using MaxSATSolver::MaxSATSolver;
    GLSSolver(const SATInstance &instance, uint seed = time(NULL));

    virtual void solve();

    void print_solution();

  private:
    uint seed;
    int trials = 0;
    double param = 1;

    // Maps a variable to the clauses it appears in
    vector<vector<int>> affected_clauses;

    // Penality for each variable
    vector<int> penalty;

    priority_queue<pair<double, int>> utility;

    int evaluate_guided_flip(vector<bool> &assignment, int flipped_var);
    bool indicator(vector<bool> &assignment, int i);
};

#endif