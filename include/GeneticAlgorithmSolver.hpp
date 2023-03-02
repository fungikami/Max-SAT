/**
 * Implementation of a Max-SAT solver based on genetic algorithm
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#ifndef GENETICALGORITHMSOLVER_H
#define GENETICALGORITHMSOLVER_H

#define MAX_GENS 1000

#include <time.h>
#include <vector>

#include "../include/MaxSATSolver.hpp"

/**
 * @brief A MaxSAT solver based on genetic algorithm
 */
class GeneticAlgorithmSolver : public MaxSATSolver {
    public:
        using MaxSATSolver::MaxSATSolver;

        GeneticAlgorithmSolver(
            const SATInstance &instance,
            int population_size,
            int max_stagnation,
            int tournament_size,
            int mutation_percent,
            uint seed = time(NULL)
        );

        virtual void solve();

        void print_solution();

        uint seed;
        uint population_size;
        int generation = 0;
        int max_stagnation;
        int tournament_size;
        int mutation_percent;

        vector<vector<bool>> population;
        vector<int> fitness;

        void cross(
            const vector<bool> &parent1,
            const vector<bool> &parent2,
            vector<bool> &child1,
            vector<bool> &child2
        );
        void mutate(vector<bool> &solution);
        vector<bool> tournament_selection();
};

#endif