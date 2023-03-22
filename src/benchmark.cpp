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
    // LocalSearchSolver ls_solver(instance);
    // double ls_solver_t = measure_time([&] { ls_solver.solve(); });
    // ls_solver.verify_solution();
    // cout << ls_solver.optimal_n_satisfied << "\t" << ls_solver_t << "\t";

    // ========== GUIDED LOCAL SEARCH ==========
    // GLSSolver gls_solver(instance, ls_solver.seed);
    // double gls_solver_t = measure_time([&] { gls_solver.solve(); });
    // gls_solver.verify_solution();
    // cout << gls_solver.optimal_n_satisfied << "\t" << gls_solver_t << "\t";

    // ========== SIMULATED ANNEALING ==========
    // double cooling_factor = argc >= 3 ? stod(argv[2]) : 0.01;
    // SimulatedAnnealingSolver sa_solver(instance, cooling_factor);
    // double sa_solver_t = measure_time([&] { sa_solver.solve(); });
    // sa_solver.verify_solution();

    // Initial Temperature	Cooling Factor	Optimal found (SA)	Time (SA)
    // cout << sa_solver.initial_temperature << "\t" << sa_solver.cooling_factor << "\t";
    // cout << sa_solver.optimal_n_satisfied << "\t" << sa_solver_t << "\t";

    // ========== GENETIC ALGORITHM ==========
    // int population_size = argc >= 4 ? stoi(argv[3]) : 100;
    // int tournament_size = argc >= 5 ? stoi(argv[4]) : 5;
    // int mutation_probability = argc >= 6 ? stoi(argv[5]) : 60;
    // int mutation_percent = argc >= 7 ? stoi(argv[6]) : 1;
    // int elite_percent = argc >= 8 ? stoi(argv[7]) : 5;

    // GeneticAlgorithmSolver ga_solver(
    //     instance, population_size, tournament_size, 
    //     mutation_probability, mutation_percent, elite_percent
    // );
    // double ga_solver_t = measure_time([&] { ga_solver.solve(); });
    // ga_solver.verify_solution();

    // #Population	#Tournament	Prob. Mutacion	Porcentaje Mutacion	Porcentaje Elite	Optimal found (GA)	Time (GA)
    // cout << ga_solver.population_size << "\t" << ga_solver.tournament_size << "\t";
    // cout << ga_solver.mutation_probability << "\t" << ga_solver.mutation_percent << "\t";
    // cout << ga_solver.elite_percent << "\t" << ga_solver.optimal_n_satisfied << "\t" << ga_solver_t << endl;

    // ========== MEMETIC ALGORITHM ==========
    MemeticAlgorithmSolver ma_solver(instance, 100, 5, 60, 1, 5);
    double ma_solver_t = measure_time([&] { ma_solver.solve(); });
    ma_solver.verify_solution();

    // Filename	Seed	#Vars	#Clauses
    cout << filename << "\t" << ma_solver.seed << "\t" << instance.n_vars << "\t";
    cout << instance.n_clauses << "\t";

    // #Population	#Tournament	Prob. Mutacion	Porcentaje Mutacion	Porcentaje Elite	Optimal found (MA)	Time (MA)
    cout << ma_solver.population_size << "\t" << ma_solver.tournament_size << "\t";
    cout << ma_solver.mutation_probability << "\t" << ma_solver.mutation_percent << "\t";
    cout << ma_solver.elite_percent << "\t" << ma_solver.optimal_n_satisfied << "\t" << ma_solver_t << "\t";

    // ========== ANT COLONY OPTIMIZATION ==========
    ACOSolver aco_solver(instance, 100, 0.7, 0.3, 0.25, 15, 10, ma_solver.seed);
    double aco_solver_t = measure_time([&] { aco_solver.solve(); });
    aco_solver.verify_solution();

    // #Ants	Alpha	Beta	Rho Q0  Tau0 
    cout << aco_solver.n_ants << "\t" << aco_solver.alpha << "\t";
    cout << aco_solver.beta << "\t" << aco_solver.rho << "\t";
    cout << aco_solver.q0 << "\t" << aco_solver.tau0 << "\t";
    cout << aco_solver.optimal_n_satisfied << "\t" << aco_solver_t << endl;

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