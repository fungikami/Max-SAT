/**
 * A SAT problem instance
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/SATInstance.hpp"

using namespace std;

/**
 * @brief Parse a SAT instance in DIMACS format from a file
 * 
 * @param filename string with the path to the file
 */
SATInstance::SATInstance(string filename) {
    ifstream file;
    file.open(filename);

    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        // Skip comments at the beginning of the file
        if (line[0] == 'c') continue;

        // Read the number of variables and clauses
        if (line[0] == 'p') {
            sscanf(line.c_str(), "p cnf %d %d", &n_vars, &n_clauses);
            continue;
        }

        vector<int> clause;
        int literal;

        while (sscanf(line.c_str(), "%d", &literal) == 1) {
            // End of clause
            if (literal == 0) break;

            /*
             * Map the literal
             *  x -> 2x-2
             * !x -> 2x-1
             * So negated literals are odd and non-negated literals are even
             * Eases bitwise manipulations
             */
            if (literal > 0) clause.push_back((literal-1) * 2);
            else clause.push_back(-2*literal - 1);

            line = line.substr(line.find(" ") + 1);
        }
        clauses.push_back(clause);
    }

    file.close();
}

/**
 * @brief Check if a given literal is satisfied by an assignment
 * 
 * @param assignment vector of booleans with the assignment
 * @return true if the assignment satisfies the instance
 *   false otherwise
 */
bool SATInstance::is_literal_true(int literal, const vector<bool> &assignment) {
    bool var_assignment = assignment[literal>>1];

    // literal & 1 is true if literal is negated
    return literal & 1 ? !var_assignment : var_assignment;
}

/**
 * @brief Print the SAT instance in a DIMACS wcnf format
 */
ostream& operator<<(ostream &os, const SATInstance &instance) {
    // Prints the header
    os << "c Ka Fung & Christopher Gómez, 2022" << endl;
    os << "p cnf " << instance.n_vars << " " << instance.n_clauses << endl;

    // Prints the clauses
    vector<vector<int>> clauses = instance.clauses;
    for (uint i=0; i<instance.clauses.size(); i++) {
        for (int literal : clauses[i]) {
            // Unmaps variables to their original form
            if (literal & 1) os << "-";
            os << ((literal>>1) +1) << " ";
        }
        os << "0" << endl;
    }

    return os;
}