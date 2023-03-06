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
    // Param: cooling_factor, seed
    // 4sat_1000_10000_50
    // temp 1, cool fact 0.999, exp cool, 9971
    // temp 1, cool fact 0.01, log cool, 9973, 0.24396s
    // temp 10, cool fact 0.01, log cool, 9974, 0.271507s
    // temp 100, cool fact 0.01, log cool, 9973, 0.231603s
    // temp auto, cool fact 0.01, log cool, 9974, 0.240004s
    // 4sat_300_2500_35 
    // temp auto, cool fact 0.01, log cool, 2500, 0.029113s
    SimulatedAnnealingSolver sa_solver(instance, 0.01);
    double sa_solver_t = measure_time([&] { sa_solver.solve(); });
    cout << "c SA " << sa_solver_t << "s" << endl;
    sa_solver.print_solution();
    sa_solver.verify_solution();

    // ========== GENETIC ALGORITHM ==========
    /* Param: population_size, tournament_size, 
     *      mutation_probability, mutation_percent, elite_rate
     * Con 4sat_1000_10000_50
     *  Elite: 0.2, 9857/9860/9868/9870. Elite: 0.1, 9848/9858/9870
     *  Elite: 0.5, 9854/9857/9876. Elite: 0.3 9853/9858/clearl
     
     */
    GeneticAlgorithmSolver ga_solver(instance, 100, 5, 60, 1, 5);
    double ga_solver_t = measure_time([&] { ga_solver.solve(); });
    cout << "c GA " << ga_solver_t << "s" << endl;
    ga_solver.print_solution();
    ga_solver.verify_solution();

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