/**
 * Implementation of a MaxSAT solver with brute force
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */
#ifndef BRUTEFORCESOLVER_H
#define BRUTEFORCESOLVER_H

#include "../include/SATSolver.hpp"

/**
 * @brief A MaxSAT solver
 * 
 */
class BruteForceSolver : public SATSolver {
  public:
    using SATSolver::SATSolver;

    virtual void solve();
    void solve_helper(int i);
};

#endif