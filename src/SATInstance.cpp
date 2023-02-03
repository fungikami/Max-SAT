/**
 * Solver de Max-SAT
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

// TODO
// Hacer que el parser vea los pesos si dice wcnf

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../include/SATInstance.hpp"

using namespace std;

/**
 * @brief Parse a SAT instance from a file
 * 
 * @param filename string with the path to the file
 */
SATInstance::SATInstance(string filename) {
    ifstream file;
    file.open(filename);

    // Check if the file is open
    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }

    // Skip comments at the beginning of the file
    string line;
    while (getline(file, line)) {
        if (line[0] == 'c') continue;

        // Read the number of variables and clauses
        if (line[0] == 'p') {
            // Check if the instances is weighted
            if (line[2] == 'w') {
                is_weighted = true;

                sscanf(line.c_str(),"p wcnf %d %d", &n_vars, &n_clauses);
            }
            else {
                sscanf(line.c_str(), "p cnf %d %d", &n_vars, &n_clauses);
            } 

            continue;
        }

        // Store the clauses
        vector<int> clause;
        int literal;

        bool weight_read = false;
        while (sscanf(line.c_str(), "%d", &literal) == 1) {

            if (literal == 0) {
                weight_read = false;
                break;
            }

            if (!weight_read) {
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
    
    // Close the file
    file.close();
}

/**
 * @brief Print the SAT instance in a DIMACS-like format
 */
ostream& operator<<(ostream &os, const SATInstance &sat_instance) {
    // Print the metadata
    os << sat_instance.n_vars << " variables" << endl;
    os << sat_instance.n_clauses << " clauses" << endl;
    
    if (sat_instance.is_weighted) {
        os << "Weighted instance" << endl;
    }
    os << endl;

    // Print every clause
    vector<vector<int>> clauses = sat_instance.clauses;
    for (uint i=0; i<clauses.size(); i++) {
        if (sat_instance.is_weighted) os << sat_instance.weights[i] << " ";

        for (uint j=0; j<clauses[i].size(); j++) {

            // Unmaps variables to their original form
            if (clauses[i][j] % 2) os << "-" << (clauses[i][j]+1)/2;
            else os << clauses[i][j]/2 + 1;

            os << " ";
        }
        os << endl;
    }

    return os;
}