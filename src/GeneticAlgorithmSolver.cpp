/**
 * Implementation of a Max-SAT solver based on genetic algorithm
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <iostream>

#include "../include/SATInstance.hpp"
#include "../include/GeneticAlgorithmSolver.hpp"

/**
 * @brief Generates an initial population for the instance to be solved, using
 * the given seed
 * 
 * @param instance The SAT instance
 * @param population_size The size of the population
 * @param tournament_size Size of the tournament
 * @param mutation_percent Percentage of mutation of each child
 * @param seed The seed for the random number generator
 * @return GeneticAlgorithmSolver:: 
 */
GeneticAlgorithmSolver::GeneticAlgorithmSolver(
    const SATInstance &instance,
    int population_size,
    int tournament_size,
    int mutation_probability,
    int mutation_percent,
    uint seed
) : MaxSATSolver(instance),
    seed(seed),
    population_size(population_size),
    tournament_size(tournament_size),
    mutation_probability(mutation_probability),
    mutation_percent(mutation_percent)
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
}

/* ========== IDEAS ==========
- Ver si se pueden tener padres ordenados por fitness o algo asi (?)
- Seleccionar solo el mejor de los dos hijos y dejar el mejor padre (para que
  la población no quede siempre enteramente sustituida) o elegir mejores entre
  los 4
- Más de 2 hijos (o padres)
- Implementar elitismo
- Cruce verificando que cierta clausula se satisfaga
- Diversidad o fitness promedio como criterio de terminación
- Paralelizar jojononojojononononojojononononojojonononojojo
============================== */

/**
 * @brief Solves the instance using genetic algorithm
 */
void GeneticAlgorithmSolver::solve() {
    while (generation < MAX_GENS) {
        vector<vector<bool>> new_population;
        vector<int> new_fitness;
        while (new_population.size() < population_size) {
            // Generate two parents
            vector<bool> parent1 = tournament_selection();
            vector<bool> parent2 = tournament_selection();

            // Generate two children from the parents
            vector<bool> child1, child2;
            cross(parent1, parent2, child1, child2);

            // Mutate the children with a given probability
            if (rand() % 100 < mutation_probability) {
                mutate(child1);
                mutate(child2);
            }

            // Compute the fitness of the new children
            int child1_fitness = compute_n_satisfied(child1);
            int child2_fitness = compute_n_satisfied(child2);

            // Update the optimal solution if a new one is found
            if (child1_fitness > optimal_n_satisfied) {
                optimal_n_satisfied = child1_fitness;
                optimal_assignment = child1;
            }

            if (child2_fitness > optimal_n_satisfied) {
                optimal_n_satisfied = child2_fitness;
                optimal_assignment = child2;
            }

            if (optimal_n_satisfied == instance.n_clauses)
                optimal_found = true;

            // Add the children to the new population
            new_population.push_back(child1);
            new_fitness.push_back(child1_fitness);

            new_population.push_back(childurnament_selection();
            vector<bool> parent2 = tournament_selection();2);
            new_fitness.push_back(child2_fitness);
        }
        if (optimal_found) break;

        population = new_population;
        fitness = new_fitness;

        generation++;
    }
}

/**
 * @brief Cross two solutions
 *
 * @param parent1 The first solution
 * @param parent2 The second solution
 * @param child1 [out] The first child
 * @param child2 [out] The second child
 */
void GeneticAlgorithmSolver::cross(
    const vector<bool> &parent1,
    const vector<bool> &parent2,
    vector<bool> &child1,urnament_selection();
            vector<bool> parent2 = tournament_selection();
) {
    // Choose a random point to cross
    int cross_point = rand() % instance.n_vars;

    // First children gets the first part of the first parent and the second
    // part of the second parent; and viceversa for the second child
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
        if (rand() % 100 < mutation_percent)
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

/**
 * @brief Roulette Wheel Selection
 *
 * @return vector<bool> The selected solution
 */
vector<bool> GeneticAlgorithmSolver::roulette_wheel_selection() {
    // Calculate the total fitness of the population
    int total_fitness = 0;
    for (uint i = 0; i < population_size; i++) total_fitness += fitness[i];

    // Calculate the probability of each solution
    vector<double> probabilities;
    for (uint i = 0; i < population_size; i++)
        probabilities.push_back((double) fitness[i] / total_fitness);

    // Choose a random number between 0 and 1
    double random = (double) rand() / RAND_MAX;

    // Choose the solution that corresponds to the random number
    double sum = 0;
    for (uint i = 0; i < population_size; i++) {
        sum += probabilities[i];
        if (sum >= random)
            return population[i];
    }

    // If the random number is 1, return the last solution
    return population[population_size - 1];
}

/**
 * @brief Elitist Selection
 *
 * @return vector<bool> The selected solution
 */
vector<bool> GeneticAlgorithmSolver::elitist_selection() {
    vector<bool> best_solution;
    int best_fitness = -1;

    // Choose the best solution
    for (uint i = 0; i < population_size; i++) {
        if (fitness[i] > best_fitness) {
            best_fitness = fitness[i];
            best_solution = population[i];
        }
    }
    return best_solution;
}

void GeneticAlgorithmSolver::print_solution() {
    cout << "c seed = " << seed << endl;
    MaxSATSolver::print_solution();
}
