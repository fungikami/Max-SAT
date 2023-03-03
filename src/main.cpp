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

    // ========== BRUTE FORCE ==========
    // cout << "Exact Algorithm" << endl;
    // BruteForceSolver bf_solver(instance);
    // double bf_solver_t = measure_time([&] { bf_solver.solve(); });
    // bf_solver.print_solution();
    // bf_solver.verify_solution();

    // ========== LOCAL SEARCH ==========
    // LocalSearchSolver ls_solver(instance);
    // double ls_solver_t = measure_time([&] { ls_solver.solve(); });
    // cout << "\nLocal Search" << endl;
    // ls_solver.print_solution();
    // ls_solver.verify_solution();

    // ========== GUIDED LOCAL SEARCH ==========
    // GLSSolver gls_solver(instance, ls_solver.seed);
    // double gls_solver_t = measure_time([&] { gls_solver.solve(); });
    // cout << "\nGLS" << endl;
    // gls_solver.print_solution();
    // gls_solver.verify_solution();

    // ========== SIMULATED ANNEALING ==========
    // Param: max_stagnation, max_no_improvement, initial_temperature
    SimulatedAnnealingSolver sa_solver(instance, 100, 100, 1);
    double sa_solver_t = measure_time([&] { sa_solver.solve(); });
    // cout << "\nSimulated Annealing" << endl;
    // sa_solver.print_solution();
    sa_solver.verify_solution();

    // ========== GENETIC ALGORITHM ==========
    /*
     * Resultados para cada porcentaje de mutacion (
     *     test4.cnf, seed 0, 100 individuos, 1000 generaciones,
     *     100 estancamiento, 5 tamano torneo
     * )
     * 
     * 0: 1102
     * 1: 1160
     * 2: 1142
     * 10: 1094
     * 500: 1083
     * 100: 1093
     */
    // GeneticAlgorithmSolver ga_solver(instance, 100, 100, 5, 1, 0);
    // double ga_solver_t = measure_time([&] { ga_solver.solve(); });
    // cout << "\nGenetic Algorithm" << endl;
    // ga_solver.print_solution();
    // ga_solver.verify_solution();

    // ========== PRINT RESULTS ==========
    // cout << "Time elapsed:" << endl;
    // // cout << "  Exact solver: " << bf_solver_t << "s" << endl;
    // cout << "       Local search: " << ls_solver_t << "s" << endl;
    // cout << "                GLS: " << gls_solver_t << "s" << endl;
    // cout << "  Simulated annealing: " << sa_solver_t << "s" << endl;
    // cout << "  Genetic algorithm: " << ga_solver_t << "s" << endl;

    // Filename	Seed	#Vars	#Clauses	Optimal found (LS)	Time (LS)	Optimal found (GLS)	Time (GLS)
    cout << filename << "\t" << sa_solver.seed << "\t" << instance.n_vars << "\t" << instance.n_clauses << "\t";
    // cout << ls_solver.optimal_n_satisfied << "\t" << ls_solver_t << "\t";
    // cout << gls_solver.optimal_n_satisfied << "\t" << gls_solver_t << "\t";
    cout << sa_solver.optimal_n_satisfied << "\t" << sa_solver_t << endl;

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