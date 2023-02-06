/**
 * Implementation of a Max-SAT solver
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */
 
#include <vector>
#include <iostream>

#include "../include/SATSolver.hpp"
#include "../include/SATInstance.hpp"

using namespace std;

/**
 * @brief Computes the weight given an assignment according to the SAT instance
 * 
 * @param assignment The assignment to be evaluated
 */
int SATSolver::compute_weight(const vector<bool> &assignment, int &n_satisfied) {
    int weight = 0;
    n_satisfied = 0;

    // For each clause
    for (int j = 0; j < instance.n_clauses; j++) {
        bool satisfied = false;

        // For each variable in the clause
        for (auto literal : instance.clauses[j]) {
            satisfied = instance.is_literal_true(literal, assignment);

            // If the clause is satisfied, skip to the next one
            if (satisfied) {
                weight += instance.weights[j];
                n_satisfied++;
                break;
            }
        }
    }

    return weight;
}

/**
 * @brief Print the solution found (may be approximate)
 * 
 * "s" line indicates if the optimal solution was known to be found or not
 * "o" line indicates the optimal weight 
 * "v" line indicates the optimal assignment
 */
void SATSolver::print_solution() {
    // Print the optimal assignment
    string solution = optimal_found ? "OPTIMUM FOUND" : "UNKNOWN";
    cout << "s " << solution << endl;
    cout << "o " << optimal_weight << endl;
    cout << "v ";

    for (uint i = 0; i < optimal_assignment.size(); i++)
        cout << (optimal_assignment[i] ? "" : "-") << (i+1) << " ";
    cout << endl;
}

/**
 * @brief Verify that the weight of the solution found is correct
 */
void SATSolver::verify_solution() {
    int n_satisfied;
    int computed_weight = compute_weight(optimal_assignment, n_satisfied);
    if (computed_weight != optimal_weight) {
        cout << "ERROR: Found solution is inconsistent" << endl;
        cout << "Reported weight is " << optimal_weight;
        cout << ", actual weight is " << computed_weight << endl;
    }
}