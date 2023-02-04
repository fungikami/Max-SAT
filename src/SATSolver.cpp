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
SATSolver::SATSolver(const SATInstance &instance) : instance(instance) {}

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
    cout << "o " << max_weight << endl;
    cout << "v ";

    for (uint i = 0; i < optimal_assignment.size(); i++)
        cout << (optimal_assignment[i] ? "" : "-") << (i+1) << " ";
    cout << endl;
}