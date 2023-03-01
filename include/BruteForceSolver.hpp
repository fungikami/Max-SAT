/**
 * Implementation of a MaxSAT solver with brute force
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef BRUTEFORCESOLVER_H
#define BRUTEFORCESOLVER_H

#include "../include/MaxSATSolver.hpp"

/**
 * @brief A MaxSAT solver
 */
class BruteForceSolver : public MaxSATSolver {
  public:
    using MaxSATSolver::MaxSATSolver;
    BruteForceSolver(const SATInstance &instance);

    virtual void solve();

    void solve_helper(vector<bool> &assignment, int i);
};

#endif