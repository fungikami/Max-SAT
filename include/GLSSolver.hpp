/**
 * Implementation of a MaxSAT solver based on guided local search
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef GLSSOLVER_H
#define GLSSOLVER_H

#include "../include/SATSolver.hpp"

/**
 * @brief A MaxSAT solver based on guided local search
 */
class GLSSolver : public SATSolver {
  public:
    using SATSolver::SATSolver;

    virtual void solve();
};

#endif