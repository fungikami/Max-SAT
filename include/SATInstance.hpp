/**
 * Implementation of a SAT instance
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */
#ifndef SATINSTANCE_H
#define SATINSTANCE_H

#include <string>
#include <vector>

using namespace std;

/**
 * @brief A SAT instance
 */
class SATInstance {
  public:
    // Constructor
    SATInstance(string filename);

    // Metadata
    int n_vars;
    int n_clauses;

    // Clauses as arrays of vectors
    vector<vector<int>> clauses;

    // Weights of clauses
    bool is_weighted = false;
    vector<int> weights;

};

ostream& operator<<(ostream &os, const SATInstance &satInstance);

#endif