/**
 * Implementation of a Max-SAT solver
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */
 
#include <vector>
#include <iostream>

#include "../include/MaxSATSolver.hpp"
#include "../include/SATInstance.hpp"

using namespace std;

MaxSATSolver::MaxSATSolver(const SATInstance &instance)
    : instance(instance), affected_clauses(instance.n_vars) {
    optimal_found = false;
    optimal_n_satisfied = 0;

    // For each clause, save for each variable the clauses it affects
    for (int i = 0; i < instance.n_clauses; i++)
        for (auto literal : instance.clauses[i])
            affected_clauses[literal>>1].push_back(i);
}

/**
 * @brief Computes the n_satisfied_clauses given an assignment according to the 
 * SAT instance
 * 
 * @param assignment The assignment to be evaluated
 */
int MaxSATSolver::compute_n_satisfied(const vector<bool> &assignment) {
    int n_satisfied = 0;

    // For each clause
    for (int j = 0; j < instance.n_clauses; j++) {
        // For each variable in the clause
        for (auto literal : instance.clauses[j]) {
            // If the clause is satisfied, skip to the next one
            if (instance.is_literal_true(literal, assignment)) {
                n_satisfied++;
                break;
            }
        }
    }

    return n_satisfied;
}

/**
 * @brief Print the solution found (may be approximate)
 * 
 * "s" line indicates if the optimal solution was known to be found or not
 * "o" line indicates the optimal number of satisfied clauses
 * "v" line indicates the optimal assignment
 */
void MaxSATSolver::print_solution() {
    // Print the optimal assignment
    cout << "c Ka Fung & Christopher Gómez (c) 2023 " << endl;
    string solution = optimal_found ? "OPTIMUM FOUND" : "UNKNOWN";
    cout << "s " << solution << endl;
    cout << "o " << optimal_n_satisfied << endl;
    cout << "v ";

    for (uint i = 0; i < optimal_assignment.size(); i++)
        cout << (optimal_assignment[i] ? "" : "-") << (i+1) << " ";
    cout << endl;
}

/**
 * @brief Verify that the number of sastified clauses of the solution found 
 * is correct
 */
void MaxSATSolver::verify_solution() {
    int computed_n_sastified = compute_n_satisfied(optimal_assignment);
    if (computed_n_sastified != optimal_n_satisfied) {
        cout << "ERROR: Found solution is inconsistent" << endl;
        cout << "Reported number of sastified clauses is " << optimal_n_satisfied;
        cout << ", actual number of sastified clauses is " << computed_n_sastified;
        cout << endl;
    }
}