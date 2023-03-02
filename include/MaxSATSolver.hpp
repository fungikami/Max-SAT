/**
 * Implementation of a Max-SAT solver
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef SATSOLVER_H
#define SATSOLVER_H

#include <vector>

#include "../include/SATInstance.hpp"

using namespace std;

/**
 * @brief A Max-SAT solver
 */
class MaxSATSolver {
  public:
    MaxSATSolver(const SATInstance &instance);

    virtual void solve() = 0;

    void print_solution();
    void verify_solution();

    int compute_n_satisfied(const vector<bool> &assignment);

    SATInstance instance;

    int optimal_n_satisfied;
    bool optimal_found;
    vector<bool> optimal_assignment;

    // Maps a variable to the clauses it appears in
    vector<vector<int>> affected_clauses;
};

#endif