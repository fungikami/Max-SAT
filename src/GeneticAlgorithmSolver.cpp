/**
 * Implementation of a Max-SAT solver based on genetic algorithm
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <iostream>
#include "../include/GeneticAlgorithmSolver.hpp"
#include "../include/SATInstance.hpp"

GeneticAlgorithmSolver:: GeneticAlgorithmSolver(
    const SATInstance &instance,
    int population_size,
    int max_generations,
    int max_stagnation,
    int tournament_size,
    double mutation_rate,
    uint seed
) : MaxSATSolver(instance),
    seed(seed),
    population_size(population_size),
    max_generations(max_generations),
    max_stagnation(max_stagnation),
    tournament_size(tournament_size),
    mutation_rate(mutation_rate)
{
    // Initialize the population with random solutions
    srand(seed);

    // Generate population_size random solutions
    for (int i = 0; i < population_size; i++) {
        vector<bool> solution;
        for (int i = 0; i < instance.n_vars; i++) solution.push_back(rand() & 1);
        population.push_back(solution);
    }

    // Calculate fitness of each solution
    for (int i = 0; i < population_size; i++)
        fitness.push_back(compute_n_satisfied(population[i]));
};


/**
 * @brief Solves the instance using genetic algorithm
 */
void GeneticAlgorithmSolver::solve() {
    int generation = 0;
    int stagnation = 0;
    while (generation < max_generations && stagnation < max_stagnation) {
        // Reproduction phase
        vector<vector<bool>> new_population;
        for (int i = 0; i < population_size; i++) {
            vector<bool> parent1 = tournament_selection();
            vector<bool> parent2 = tournament_selection();
            vector<bool> child1, child2;
            cross(parent1, parent2, child1, child2);
            mutate(child1);
            mutate(child2);
            new_population.push_back(child1);
            new_population.push_back(child2);
        }
        population = new_population;

        int current_best_fitness = 0;
        for (int i = 0; i < population_size; i++) {
            int fitness = compute_n_satisfied(population[i]);
            if (fitness > current_best_fitness)
                current_best_fitness = fitness;
        }
        if (current_best_fitness > optimal_n_satisfied) {
            optimal_n_satisfied = current_best_fitness;
            stagnation = 0;
        } else {
            stagnation++;
        }
        generation++;
    }

    optimal_assignment = population[0];
    
}

/**
 * @brief Cross two solutions
 *
 * @param parent1 The first solution
 * @param parent2 The second solution
 * @param child1 The first child
 * @param child2 The second child
 */
void GeneticAlgorithmSolver::cross(
    const vector<bool> &parent1,
    const vector<bool> &parent2,
    vector<bool> &child1,
    vector<bool> &child2
) {
    int cross_point = rand() % instance.n_vars;
    for (int i = 0; i < cross_point; i++) {
        child1.push_back(parent1[i]);
        child2.push_back(parent2[i]);
    }
    for (int i = cross_point; i < instance.n_vars; i++) {
        child1.push_back(parent2[i]);
        child2.push_back(parent1[i]);
    }
}

/**
 * @brief Mutate a solution
 *
 * @param solution The solution to mutate
 */
void GeneticAlgorithmSolver::mutate(vector<bool> &solution) {
    for (int i = 0; i < instance.n_vars; i++) {
        if (rand() % 100 < mutation_rate * 100)
            solution[i] = !solution[i];
    }
}

/**
 * @brief Tournament selection
 *
 * @return vector<bool> The selected solution
 */
vector<bool> GeneticAlgorithmSolver::tournament_selection() {
    vector<bool> best_solution;
    int best_fitness = 0;
    for (int i = 0; i < tournament_size; i++) {
        int index = rand() % population_size;
        int fitness = compute_n_satisfied(population[index]);
        if (fitness > best_fitness) {
            best_fitness = fitness;
            best_solution = population[index];
        }
    }
    return best_solution;
}

void GeneticAlgorithmSolver::print_solution() {
    cout << "c seed = " << seed << endl;
    MaxSATSolver::print_solution();
}
