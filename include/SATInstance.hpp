/**
 * A SAT problem instance
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
    SATInstance(string filename);

    int n_vars;
    int n_clauses;
    int max_weight = 0;

    vector<vector<int>> clauses;
    vector<int> weights;

    bool is_literal_true(int literal, const vector<bool> &assignment);
};

ostream& operator<<(ostream &os, const SATInstance &satInstance);

#endif