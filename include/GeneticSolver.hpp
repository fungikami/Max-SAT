/**
 * Implementation of a Max-SAT solver based on genetic algorithm
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

// #ifndef GENETICSOLVER_H
// #define GENETICSOLVER_H

// #include "../include/MaxSATSolver.hpp"

// /**
//  * @brief A MaxSAT solver based on genetic algorithm
//  */
// class GeneticSolver : public MaxSATSolver {
//     public:
//         using MaxSATSolver::MaxSATSolver;
//         GeneticSolver(const SATInstance &instance, uint seed = time(NULL));
    
//         virtual void solve();
    
//         void print_solution();
    
//     private:
//         uint seed;
//         int population_size = 100;
//         int max_generations = 1000;
//         double mutation_rate = 0.1;
    
//         vector<vector<bool>> population;
//         vector<int> fitness;
    
//         vector<bool> generate_random_solution();
//         int calculate_fitness(vector<bool> solution);
//         void cross(vector<bool> parent1, vector<bool> parent2, vector<bool> &child1, vector<bool> &child2);
//         void mutate(vector<bool> &solution);
//         void select();
// };

// #endif