/**
 * Solver de Max-SAT
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <functional>
#include <iostream>
#include <string>
#include <chrono>

#include "../include/BruteForceSolver.hpp"
#include "../include/LocalSearchSolver.hpp"
#include "../include/GLSSolver.hpp"
#include "../include/GeneticAlgorithmSolver.hpp"
#include "../include/SATInstance.hpp"

using namespace std;

double measure_time(function<void()> f);

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
    string filename = argv[1];

    // ========== MAXSAT INSTANCE ==========
    SATInstance instance(filename);

    // ========== BRUTE FORCE ==========
    // cout << "Exact Algorithm" << endl;
    // BruteForceSolver bf_solver(instance);
    // double bf_solver_t = measure_time([&] { bf_solver.solve(); });
    // bf_solver.print_solution();
    // bf_solver.verify_solution();

    // ========== LOCAL SEARCH ==========
    LocalSearchSolver ls_solver(instance);
    double ls_solver_t = measure_time([&] { ls_solver.solve(); });
    // cout << "\nLocal Search" << endl;
    ls_solver.print_solution();
    // ls_solver.verify_solution();

    // ========== GUIDED LOCAL SEARCH ==========
    GLSSolver gls_solver(instance, ls_solver.seed);
    double gls_solver_t = measure_time([&] { gls_solver.solve(); });
    // cout << "\nGLS" << endl;
    gls_solver.print_solution();
    // gls_solver.verify_solution();

    // ========== GENETIC ALGORITHM ==========
    GeneticAlgorithmSolver ga_solver(instance, 100, 1000, 100, 5, 0.1, ls_solver.seed);
    double ga_solver_t = measure_time([&] { ga_solver.solve(); });
    // cout << "\nGenetic Algorithm" << endl;
    ga_solver.print_solution();
    ga_solver.verify_solution();

    // ========== PRINT RESULTS ==========
    cout << "Time elapsed:" << endl;
    // cout << "  Exact solver: " << bf_solver_t << "s" << endl;
    cout << "       Local search: " << ls_solver_t << "s" << endl;
    cout << "                GLS: " << gls_solver_t << "s" << endl;
    cout << "  Genetic algorithm: " << ga_solver_t << "s" << endl;

    // Filename	#Vars	#Clauses	Seed	Optimal found (LS)	Time (LS)	Optimal found (GLS)	Time (GLS)
    // cout << filename << "\t" << instance.n_vars << "\t" << instance.n_clauses << "\t";
    // cout << ls_solver.seed << "\t" << ls_solver.optimal_n_satisfied << "\t" << ls_solver_t << "\t";
    // cout << gls_solver.optimal_n_satisfied << "\t" << gls_solver_t << endl;

    return 0;
}

/**
 * @brief Function to measure the time elapsed executing a function
 * 
 * @param f The function to be executed
 * @return double The time elapsed in seconds
 */
double measure_time(function<void()> f) {
    auto start = chrono::high_resolution_clock::now();
    f();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::duration<double>>(end - start).count();
}