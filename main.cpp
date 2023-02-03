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
            sscanf(line.c_str(), "p cnf %d %d", &(this->nVars), &(this->nClauses));
            continue;
        }

        // Store the clauses
        vector<int> clause;
        int literal;
        
        string line_copy = line;

        while (sscanf(line_copy.c_str(), "%d", &literal) == 1) {
            if (literal == 0) break;

            /*
             * Map the literal
             *  x -> 2x-2
             * !x -> 2x-1
             * So negated literals are odd and non-negated literals are even
             * Eases bitwise maniputations
             */
            if (literal > 0) clause.push_back((literal-1) * 2);
            else clause.push_back(-2*literal - 1);

            line_copy = line_copy.substr(line_copy.find(" ") + 1);
        }
        clauses.push_back(clause);
    }
    
    // Close the file
    file.close();
}

/**
 * @brief Print the SAT instance in a DIMACS-like format
 */
ostream& operator<<(ostream &os, const SATInstance &satInstance) {
    // Print the metadata
    os << satInstance.nVars << " variables" << endl;
    os << satInstance.nClauses << " clauses" << endl << endl;

    // Print every clause
    vector<vector<int>> clauses = satInstance.clauses;
    for (uint i=0; i<clauses.size(); i++) {
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

    // Print the SAT instance
    cout << satInstance;

    return 0;
}