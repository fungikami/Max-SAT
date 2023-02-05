/**
 * Implementation of a Max-SAT solver
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef SATSOLVER_H
#define SATSOLVER_H

#include <vector>
#include <string>

#include "../include/SATInstance.hpp"

using namespace std;

/**
 * @brief A Max-SAT solver
 */
class SATSolver {
  public:
    SATSolver(const SATInstance &instance);

    virtual void solve() = 0;
    int compute_weight(const vector<bool> &assignment, int &n_satisfied);
    void print_solution();
    void verify_solution();

    SATInstance instance;

    int optimal_weight = -1;
    bool optimal_found = false;

    vector<bool> optimal_assignment;
    vector<vector<int>> watchlist;
};

#endif