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
        void cross(
            const vector<bool> &parent1,
            const vector<bool> &parent2,
            vector<bool> &child1,
            vector<bool> &child2
        );

        void two_point_crossover(
            const vector<bool> &parent1,
            const vector<bool> &parent2,
            vector<bool> &child1,
            vector<bool> &child2
        );

        // Mutation
        int mutate(vector<bool> &solution);
        int mutateGreedy(vector<bool> &solution);
        int eval_function(
            vector<bool> &assignment,
            int flipped_var,
            int current_n_satisfied
        );

        // Selection
        vector<bool> tournament_selection();
        vector<bool> roulette_wheel_selection();
        vector<bool> elitist_selection();

        void select_elite(
            vector<vector<bool>> &population,
            vector<int> &fitness,
            int n_elite
        );
};

#endif