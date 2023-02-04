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
    bool is_weighted = false;
    while (getline(file, line)) {
        // Skip comments at the beginning of the file
        if (line[0] == 'c') continue;

        // Read the number of variables and clauses
        if (line[0] == 'p') {
            is_weighted = line[2] == 'w';

            if (is_weighted)
                sscanf(line.c_str(),"p wcnf %d %d", &n_vars, &n_clauses);
            else
                sscanf(line.c_str(), "p cnf %d %d", &n_vars, &n_clauses);
            continue;
        }

        vector<int> clause;
        int literal;

        bool weight_read = false;
        while (sscanf(line.c_str(), "%d", &literal) == 1) {
            // Reset the weight_read boolean if the clause is finished
            if (literal == 0) {
                weight_read = false;
                break;
            }

            if (!weight_read) {
                // Push the weight of the clause
                if (is_weighted) {
                    weights.push_back(literal);
                    line = line.substr(line.find(" ") + 1);
                } else {
                    weights.push_back(1);
                }

                weight_read = true;
                continue;
            }
            
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
 * @brief Print the SAT instance in a DIMACS wcnf format
 */
ostream& operator<<(ostream &os, const SATInstance &instance) {
    // Prints the header
    os << "c Ka Fung & Christopher Gómez, 2022" << endl;
    os << "p wcnf " << instance.n_vars << " " << instance.n_clauses;

    // Prints the clauses
    vector<vector<int>> clauses = instance.clauses;
    for (uint i=0; i<instance.clauses.size(); i++) {
        os << endl << instance.weights[i] << " ";

        for (int literal : clauses[i]) {
            // Unmaps variables to their original form
            if (literal & 1) os << "-";
            os << ((literal>>1) +1) << " ";
        }
        os << "0";
    }

    return os;
}