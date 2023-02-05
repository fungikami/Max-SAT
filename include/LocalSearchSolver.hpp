/**
 * Implementation of a MaxSAT solver based on local search
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef LOCALSEARCHSOLVER_H
#define LOCALSEARCHSOLVER_H

#include "../include/SATSolver.hpp"

/**
 * @brief A MaxSAT solver based on local search
 */
class LocalSearchSolver : public SATSolver {
  public:
    using SATSolver::SATSolver;

    virtual void solve();
};

#endif