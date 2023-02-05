/**
 * Solver de Max-SAT
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <functional>
#include <iostream>
#include <chrono>

#include "../include/BruteForceSolver.hpp"
#include "../include/LocalSearchSolver.hpp"
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

    // ========== READ THE INSTANCE ==========
    SATInstance instance(argv[1]);

    // Verificar por qué local search da peores resultados si se ejecuta brute
    // force primero
    // ========== BRUTE FORCE ==========
    BruteForceSolver bf_solver(SATInstance("test_data/others/test1.cnf"));
    double bf_solver_t = measure_time([&] { bf_solver.solve(); });
    bf_solver.verify_solution();

    // ========== LOCAL SEARCH ==========
    /**
     * Buenas semillas:
     *  -test_7 = 1675590184
     *  -test_3 = 1675618427
     */
    LocalSearchSolver ls_solver(instance);
    double ls_solver_t = measure_time([&] { ls_solver.solve(); });
    // cout << ls_solver.optimal_weight << endl;
    ls_solver.verify_solution();

    // ========== GUIDED LOCAL SEARCH ==========
    // GLSSolver gls_solver(instance);
    // double gls_solver_t = measure_time([&] { gls_solver.solve(); });
    // gls_solver.verify_solution();

    // ========== PRINT RESULTS ==========
    cout << "Time elapsed:" << endl;
    cout << "  Exact solver: " << bf_solver_t << "s" << endl;
    cout << "  Local search: " << ls_solver_t << "s" << endl;
    // cout << "           GLS: " << gls_solver_t << "s" << endl;

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