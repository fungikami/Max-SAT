/**
 * Implementation of a Max-SAT solver based on memetic algorithm
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef MEMETICALGORITHMSOLVER_H
#define MEMETICALGORITHMSOLVER_H

#define MAX_GENERATIONS 100

#include <time.h>
#include <vector>

#include "../include/MaxSATSolver.hpp"

/**
 * @brief A MaxSAT solver based on memetic algorithm
 */
class MemeticAlgorithmSolver : public MaxSATSolver {
    public:
        using MaxSATSolver::MaxSATSolver;

        MemeticAlgorithmSolver(
            const SATInstance &instance,
            int population_size,
            int tournament_size,
            int mutation_probability,
            int mutation_percent,
            int elite_percent,
            uint seed = time(NULL)
        );

        virtual void solve();

        void print_solution();

        uint seed;
        uint population_size;
        int generation = 0;
        int tournament_size;
        int mutation_probability;
        int mutation_percent;
        int elite_percent;

        vector<vector<bool>> population;
        vector<int> fitness;

        // Crossover
        void uniform_recombination(
            vector<vector<bool>> &parents,
            vector<vector<bool>> &children
        );

        // Mutation
        int mutate(vector<bool> &solution);
        int eval_function(
            vector<bool> &assignment,
            int flipped_var,
            int current_n_satisfied
        );

        // Selection
        vector<bool> tournament_selection();

        void select_elite(
            vector<vector<bool>> &population,
            vector<int> &fitness,
            int n_elite
        );
};

#endif