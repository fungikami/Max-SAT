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
#include "../include/MemeticAlgorithmSolver.hpp"
#include "../include/ACOSolver.hpp"
#include "../include/SATInstance.hpp"
#include "../include/SolutionTreeSolver.hpp"

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
    // cout << "c time = " << bf_solver_t << "s" << endl;
    // bf_solver.print_solution();
    // bf_solver.verify_solution();

    // ========== LOCAL SEARCH ==========
    LocalSearchSolver ls_solver(instance);
    double ls_solver_t = measure_time([&] { ls_solver.solve(); });
    cout << "c time = " << ls_solver_t << "s" << endl;
    ls_solver.print_solution();
    ls_solver.verify_solution();

    // ========== GUIDED LOCAL SEARCH ==========
    // GLSSolver gls_solver(instance, ls_solver.seed);
    // double gls_solver_t = measure_time([&] { gls_solver.solve(); });
    // cout << "c time = " << gls_solver_t << "s" << endl;
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
    // SimulatedAnnealingSolver sa_solver(instance, 0.01);
    // double sa_solver_t = measure_time([&] { sa_solver.solve(); });
    // cout << "c time = " << sa_solver_t << "s" << endl;
    // sa_solver.print_solution();
    // sa_solver.verify_solution();

    // ========== GENETIC ALGORITHM ==========
    /* Param: population_size, tournament_size, 
     *      mutation_probability, mutation_percent, elite_rate
     * Con 4sat_1000_10000_50
     *  Elite: 0.2, 9857/9860/9868/9870. Elite: 0.1, 9848/9858/9870
     *  Elite: 0.5, 9854/9857/9876. Elite: 0.3 9853/9858/clearl
     
     */
    // GeneticAlgorithmSolver ga_solver(instance, 100, 5, 60, 1, 5);
    // double ga_solver_t = measure_time([&] { ga_solver.solve(); });
    // cout << "c time = " << ga_solver_t << "s" << endl;
    // ga_solver.print_solution();
    // ga_solver.verify_solution();

    // ========== ACO ==========
    /* Param: n_ants, alpha, beta, rho, q0, tau0,
     *
     */
    // ACOSolver aco_solver(instance, 100, 0.7, 0.3, 0.25, 15, 10);
    // double aco_solver_t = measure_time([&] { aco_solver.solve(); });
    // cout << "c time = " << aco_solver_t << "s" << endl;
    // aco_solver.print_solution();
    // aco_solver.verify_solution();

    // ========== MEMETIC ALGORITHM ==========
    // MemeticAlgorithmSolver ma_solver(instance, 100, 5, 60, 1, 5);
    // double ma_solver_t = measure_time([&] { ma_solver.solve(); });
    // cout << "c time = " << ma_solver_t << "s" << endl;
    // ma_solver.print_solution();
    // ma_solver.verify_solution();

    // ========== SOLUTION TREE ==========
    int branching_factor = stoi(argv[2]);
    int max_depth = stoi(argv[3]);

    SolutionTreeSolver st_solver(instance, branching_factor, max_depth);
    double st_solver_t = measure_time([&] { st_solver.solve(); });
    cout << "c time = " << st_solver_t << "s" << endl;
    st_solver.print_solution();
    st_solver.verify_solution();

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