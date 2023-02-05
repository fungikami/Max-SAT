#include <vector>
#include <iostream>

#include "../include/SATSolver.hpp"
#include "../include/SATInstance.hpp"

using namespace std;

/**
 * @brief Generates an initial solution for the instance to be solved
 * 
 * @param instance The SAT instance
 */
SATSolver::SATSolver(const SATInstance &instance) : instance(instance) {
    optimal_assignment = vector<bool>(instance.n_vars, false);
}

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
        for (auto var : instance.clauses[j]) {
            bool var_assignment = assignment[var>>1];

            // var & 1 is true if var is negated
            satisfied = var & 1 ? !var_assignment : var_assignment;

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
    if (compute_weight(optimal_assignment, n_satisfied) != optimal_weight)
        cout << "ERROR: Found solution is inconsistent" << endl;
}