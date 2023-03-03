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
    // BruteForceSolver bf_solver(instance);
    // double bf_solver_t = measure_time([&] { bf_solver.solve(); });
    // cout << "c BF " << bf_solver_t << "s" << endl;
    // bf_solver.print_solution();
    // bf_solver.verify_solution();

    // ========== LOCAL SEARCH ==========
    // LocalSearchSolver ls_solver(instance);
    // double ls_solver_t = measure_time([&] { ls_solver.solve(); });
    // cout << "c LS " << ls_solver_t << "s" << endl;
    // ls_solver.print_solution();
    // ls_solver.verify_solution();

    // ========== GUIDED LOCAL SEARCH ==========
    // GLSSolver gls_solver(instance, ls_solver.seed);
    // double gls_solver_t = measure_time([&] { gls_solver.solve(); });
    // cout << "c GLS " << gls_solver_t << "s" << endl;
    // gls_solver.print_solution();
    // gls_solver.verify_solution();

    // ========== SIMULATED ANNEALING ==========
    // Param: max_no_improvement, initial_temperature
    // temp 1, cool fact 0.999, exp cool
    SimulatedAnnealingSolver sa_solver(instance, 1);
    double sa_solver_t = measure_time([&] { sa_solver.solve(); });
    cout << "c SA " << sa_solver_t << "s" << endl;
    sa_solver.print_solution();
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
    // GeneticAlgorithmSolver ga_solver(instance, 100, 5, 1, 0);
    // double ga_solver_t = measure_time([&] { ga_solver.solve(); });
    // cout << "c GA " << ga_solver_t << "s" << endl;
    // ga_solver.print_solution();
    // ga_solver.verify_solution();

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