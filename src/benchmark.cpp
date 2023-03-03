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
#include "../include/SimulatedAnnealingSolver.hpp"
#include "../include/GeneticAlgorithmSolver.hpp"
#include "../include/SATInstance.hpp"

using namespace std;

double measure_time(function<void()> f);

/**
 * @brief Main function
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

    // ========== LOCAL SEARCH ==========
    LocalSearchSolver ls_solver(instance);
    double ls_solver_t = measure_time([&] { ls_solver.solve(); });
    ls_solver.verify_solution();
    // Filename	Seed	#Vars	#Clauses	Optimal found (LS)	Time (LS)	Optimal found (GLS)	Time (GLS)
    cout << filename << "\t" << ls_solver.seed << "\t" << instance.n_vars << "\t" << instance.n_clauses << "\t";
    cout << ls_solver.optimal_n_satisfied << "\t" << ls_solver_t << "\t";

    // ========== GUIDED LOCAL SEARCH ==========
    GLSSolver gls_solver(instance, ls_solver.seed);
    double gls_solver_t = measure_time([&] { gls_solver.solve(); });
    gls_solver.verify_solution();
    cout << gls_solver.optimal_n_satisfied << "\t" << gls_solver_t << "\t";

    // ========== SIMULATED ANNEALING ==========
    SimulatedAnnealingSolver sa_solver(instance, 1);
    double sa_solver_t = measure_time([&] { sa_solver.solve(); });
    sa_solver.verify_solution();
    cout << sa_solver.optimal_n_satisfied << "\t" << sa_solver_t << endl;

    // ========== GENETIC ALGORITHM ==========
    // GeneticAlgorithmSolver ga_solver(instance, 100, 5, 1, 0);
    // double ga_solver_t = measure_time([&] { ga_solver.solve(); });
    // ga_solver.verify_solution();
    // cout << ga_solver.optimal_ngatisfied << "\t" << ga_solver_t << endl;

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