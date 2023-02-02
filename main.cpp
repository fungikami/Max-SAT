/*
 * Solver de Max-SAT
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */
#include <iostream>
#include <fstream>
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
    int nVars;
    int nClauses;

    // Clauses as arrays of vectors
    vector<vector<int>> clauses;

    // Weights of clauses
    vector<int> weights;
};

// TODO
// Arreglar lo de literal para hacer operaciones bit a bit
// Hacer que el parser vea los pesos si dice wcnf
// Implementar el algoritmo de MaxSAT

/**
 * @brief A MaxSAT solver
 * 
 */
class MaxSATSolver {
  void initialize(); // to accept input and initialize
  void solve();      // to solve and show the result
};

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
            sscanf(line.c_str(), "p cnf %d %d", &nVars, &nClauses);

            cout << "nVars: " << nVars << endl;
            cout << "nClauses: " << nClauses << endl;
            continue;
        }

        // Store the clauses
        vector<int> clause;
        int literal;
        
        string line_copy = line;

        while (sscanf(line_copy.c_str(), "%d", &literal) == 1) {
            if (literal == 0) break;

            // Map the literal
            if (literal > 0) {
                literal = (literal-1) * 2;
                clause.push_back(literal);
            } else {
                // Negate the literal
                literal = -2*literal - 1;
                clause.push_back(literal);
            }
            line_copy = line_copy.substr(line_copy.find(" ") + 1);
        }
        clauses.push_back(clause);
    }
    
    // Close the file
    file.close();
}

/**
 * @brief Initialize the solver
 * 
 * @return int
 */
int main(int argc, char *argv[]) {
    // Check if the user provided a filename
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // Parse the SAT instance
    SATInstance satInstance(argv[1]);

    return 0;
}