/**
 * Implementation of a Max-SAT solver based on memetic algorithm
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <iostream>
#include <omp.h>

#include "../include/SATInstance.hpp"
#include "../include/LocalSearchSolver.hpp"
#include "../include/MemeticAlgorithmSolver.hpp"
#include "../include/SimulatedAnnealingSolver.hpp"

/**
 * @brief Generates an initial population for the instance to be solved, using
 * the given seed
 * 
 * @param instance The SAT instance
 * @param population_size The size of the population
 * @param tournament_size Size of the tournament
 * @param mutation_percent Percentage of mutation of each child
 * @param seed The seed for the random number generator
 * @return MemeticAlgorithmSolver:: 
 */
MemeticAlgorithmSolver::MemeticAlgorithmSolver(
    const SATInstance &instance,
    int population_size,
    int tournament_size,
    int mutation_probability,
    int mutation_percent,
    int elite_percent,
    uint seed
) : MaxSATSolver(instance),
    seed(seed),
    population_size(population_size),
    tournament_size(tournament_size),
    mutation_probability(mutation_probability),
    mutation_percent(mutation_percent),
    elite_percent(elite_percent)
{
    // Initialize the population with random solutions
    srand(seed);

    // Generate population_size random solutions
    population = vector<vector<bool>>(population_size);
    fitness = vector<int>(population_size);
    for (int i = 0; i < population_size; i++) {
        vector<bool> solution;
        for (int j = 0; j < instance.n_vars; j++) solution.push_back(rand() & 1);
        population[i] = solution;
        fitness[i] = compute_n_satisfied(solution);
    }
}

/**
 * @brief Solves the instance using Memetic algorithm
 */
void MemeticAlgorithmSolver::solve() {
    int elite_individuals = population_size * elite_percent / 100;

    // Take 10% of the population for recombinations
    int k = population_size * 0.05;

    // Number of children per recombination
    int n_children = k / 2;

    // Number of reproduction iterations (considering the elite individuals)
    uint iters = (population_size - elite_individuals) / n_children + 1;

    // Apply local search to initial population
    for (uint i = 0; i < population_size; i++) {
        LocalSearchSolver solver(instance);
        solver.optimal_assignment = population[i];
        solver.solve();
        population[i] = solver.optimal_assignment;
        fitness[i] = solver.optimal_n_satisfied;
    }

    while (generation < MAX_GENERATIONS) {
        vector<vector<bool>> new_population = vector<vector<bool>>(population_size);
        vector<int> new_fitness = vector<int>(population_size);

        // Fixed sized scheduling
        #pragma omp parallel for num_threads(6)
        for (uint i = 0; i < iters; i++) {
            vector<vector<bool>> tournament_population = vector<vector<bool>>(k);
            
            // Select k random individuals from the population
            for (int j = 0; j < k; j++) {
                vector<bool> parent = tournament_selection();
                tournament_population[j] = parent;
            }
            
            // Generate n children from the parents
            vector<vector<bool>> children = vector<vector<bool>>(n_children);
            vector<int> children_fitness = vector<int>(n_children);
            uniform_recombination(tournament_population, children);

            // Mutate each children with a given probability
            for (int j = 0; j < n_children; j++) 
                children_fitness[j] = mutate(children[j]);


            // Apply local search to each children
            for (int j = 0; j < n_children; j++) {
                LocalSearchSolver solver(instance);

                // Modifica que el optimal assigment inicial sea el hijo
                solver.optimal_assignment = children[j];

                solver.solve();
                children[j] = solver.optimal_assignment;
                children_fitness[j] = solver.optimal_n_satisfied;
            }

            // Add each children to the new population
            for (int j = 0; j < n_children; j++) {
                new_population[i * n_children + j] = children[j];
                new_fitness[i * n_children + j] = children_fitness[j];
            }  

            // Update the optimal solution if a new one is found
            #pragma omp critical
            {
                for (int j = 0; j < n_children; j++) {
                    if (children_fitness[j] > optimal_n_satisfied) {
                        optimal_n_satisfied = children_fitness[j];
                        optimal_assignment = children[j];
                    }
                }

                if (optimal_n_satisfied == instance.n_clauses) 
                    optimal_found = true;
            }
        }

        if (optimal_found) break;

        // Add the best parent to the new population
        select_elite(new_population, new_fitness, elite_individuals);

        population = new_population;
        fitness = new_fitness;

        generation++;
    }
}

/**
 * @brief Selects the best solution from the population using tournament
 * selection
 * 
 * @return vector<bool> The best solution
 */
void MemeticAlgorithmSolver::select_elite(
    vector<vector<bool>> &new_population,
    vector<int> &new_fitness,
    int n_elite
) {
    vector<vector<bool>> elite;
    vector<int> elite_fitness;

    for (int i = 0; i < n_elite; i++) {
        int best_fitness = -1;
        int best_index = -1;

        for (uint j = 0; j < fitness.size(); j++) {
            if (fitness[j] > best_fitness) {
                best_fitness = fitness[j];
                best_index = j;
            }
        }

        elite.push_back(population[best_index]);
        elite_fitness.push_back(best_fitness);

        population.erase(population.begin() + best_index);
        fitness.erase(fitness.begin() + best_index);
    }

    int base_index = new_population.size() - n_elite;
    for (int i = 0; i < n_elite; i++) {
        new_population[base_index + i] = elite[i];
        new_fitness[base_index + i] = elite_fitness[i];
    }
}

void MemeticAlgorithmSolver::uniform_recombination(
    vector<vector<bool>> &parents,
    vector<vector<bool>> &children
) {
    // For each variable 
    for (int i = 0; i < instance.n_vars; i++) {
        // For each children 
        for (uint j = 0; j < children.size(); j++) {
            // Choose a random parent and add its value to the children
            int parent_index = rand() % parents.size();
            children[j].push_back(parents[parent_index][i]);
        }
    }
}

/**
 * @brief Mutate a solution
 *
 * @param solution The solution to mutate
 */
int MemeticAlgorithmSolver::mutate(vector<bool> &solution) {
    if (rand() % 100 < mutation_probability) {
        for (int i = 0; i < instance.n_vars; i++) {
            if (rand() % 100 < mutation_percent)
                solution[i] = !solution[i];
        }
    }

    return compute_n_satisfied(solution);
}

/**
 * @brief Tournament selection
 *
 * @return vector<bool> The selected solution
 */
vector<bool> MemeticAlgorithmSolver::tournament_selection() {
    vector<bool> best_solution;
    int best_fitness = -1;

    // Choose tournament_size random solutions and select the best one
    for (int i = 0; i < tournament_size; i++) {
        int index = rand() % population_size;
        if (fitness[index] > best_fitness) {
            best_fitness = fitness[index];
            best_solution = population[index];
        }
    }
    return best_solution;
}

void MemeticAlgorithmSolver::print_solution() {
    cout << "c seed = " << seed << endl;
    MaxSATSolver::print_solution();
}
