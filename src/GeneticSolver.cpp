/**
 * Implementation of a Max-SAT solver based on genetic algorithm
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <iostream>
#include "../include/GeneticSolver.hpp"
#include "../include/SATInstance.hpp"

/**
 * @brief Generate a random solution
 *
 * @return vector<bool> The random solution
 */
vector<bool> GeneticSolver::generate_random_solution() {
    vector<bool> solution;
    for (int i = 0; i < instance.n_vars; i++)
        solution.push_back(rand() & 1);
    return solution;

}

/**
 * @brief Calculate fitness of a solution
 *
 * @param solution The solution to calculate the fitness
 * @return int The fitness of the solution
 */
int GeneticSolver::calculate_fitness(vector<bool> solution) {
    int fitness = 0;
    for (int i = 0; i < instance.n_clauses; i++) {
        bool clause_satisfied = false;
        for (auto literal : instance.clauses[i]) {
            if (literal & 1) {
                if (!solution[literal >> 1]) {
                    clause_satisfied = true;
                    break;
                }
            } else {
                if (solution[literal >> 1]) {
                    clause_satisfied = true;
                    break;
                }
            }
        }
        if (clause_satisfied)
            fitness += 1;
    }
    return fitness;
}

/**
 * @brief Cross two solutions
 *
 * @param parent1 The first solution
 * @param parent2 The second solution
 * @param child1 The first child
 * @param child2 The second child
 */
void GeneticSolver::cross(vector<bool> parent1, vector<bool> parent2, vector<bool> &child1, vector<bool> &child2) {
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
void GeneticSolver::mutate(vector<bool> &solution) {
    for (int i = 0; i < instance.n_vars; i++) {
        if (rand() % 100 < mutation_rate * 100)
            solution[i] = !solution[i];
    }
}

/**
 * @brief Tournament selection
 *
 * @param population The population to select from
 * @param tournament_size The size of the tournament
 * @return vector<bool> The selected solution
 */
vector<bool> GeneticSolver::tournament_selection(vector<vector<bool>> population) {
    vector<bool> best_solution;
    int best_fitness = 0;
    for (int i = 0; i < tournament_size; i++) {
        int index = rand() % population.size();
        int fitness = calculate_fitness(population[index]);
        if (fitness > best_fitness) {
            best_fitness = fitness;
            best_solution = population[index];
        }
    }
    return best_solution;
}

GeneticSolver::GeneticSolver(
    const SATInstance &instance, uint seed
) : MaxSATSolver(instance) {
    srand(seed);
}

/**
 * @brief Solves the instance using genetic algorithm
 */
void GeneticSolver::solve() {
    vector<vector<bool>> population;
    for (int i = 0; i < population_size; i++)
        population.push_back(generate_random_solution());

    int generation = 0;
    int stagnation = 0;
    int best_fitness = 0;
    while (generation < max_generations && stagnation < max_stagnation) {
        vector<vector<bool>> new_population;
        for (int i = 0; i < population_size; i++) {
            vector<bool> parent1 = tournament_selection(population);
            vector<bool> parent2 = tournament_selection(population);
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
            int fitness = calculate_fitness(population[i]);
            if (fitness > current_best_fitness)
                current_best_fitness = fitness;
        }
        if (current_best_fitness > best_fitness) {
            best_fitness = current_best_fitness;
            stagnation = 0;
        } else {
            stagnation++;
        }
        generation++;
    }

    optimal_n_satisfied = best_fitness;
    optimal_assignment = population[0];
}

void GeneticSolver::print_solution() {
    cout << "c seed = " << seed << endl;
    MaxSATSolver::print_solution();
}
