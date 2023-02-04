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
    void print_solution();

    SATInstance instance;

    int max_weight = -1;
    int optimal_found = false;

    vector<char> optimal_assignment;
    vector<vector<int>> watchlist;
};

#endif