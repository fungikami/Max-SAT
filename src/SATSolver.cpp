#include <string>
#include <iostream>

#include "../include/SATSolver.hpp"
#include "../include/SATInstance.hpp"

using namespace std;

/**
 * @brief Constructor
 * @param filename The name of the file containing the SAT instance
 */
SATSolver::SATSolver(const SATInstance &instance) : instance(instance) {      
    // Initialize the assignment and optimal assignment with -1 (not assigned)
    for (int i = 0; i < instance.n_vars; i++) {
        assignment.push_back(-1);
        optimal_assignment.push_back(-1);
    }
}

/**
 * @brief Print the solution
 * "s" line indicates if the optimal solution was found or not
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