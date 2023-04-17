/**
 * Implementation of a Max-SAT solver based on genetic algorithm
 *
 * Copyright (C) 2023 Christopher Gómez, Ka Fung
 */

#include <iostream>
#include <omp.h>

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
 * @brief Solves the instance using genetic algorithm
 */
void GeneticAlgorithmSolver::solve() {
    int elite_individuals = population_size * elite_percent / 100;
    uint iters = (population_size - elite_individuals) / 2 + 1;

    while (generation < MAX_GENS) {
        vector<vector<bool>> new_population = vector<vector<bool>>(population_size);
        vector<int> new_fitness = vector<int>(population_size);

        // Fixed sized scheduling
        #pragma omp parallel for num_threads(6)
        for (uint i = 0; i < iters; i++) {
            // Generate two parents
            vector<bool> parent1 = tournament_selection();
            vector<bool> parent2 = tournament_selection();

            // Generate two children from the parents
            vector<bool> child1, child2;
            int child1_fitness, child2_fitness;
            two_point_crossover(parent1, parent2, child1, child2);

            // Mutate the children with a given probability
            child1_fitness = mutate(child1);
            child2_fitness = mutate(child2);

            // Add the children to the new population
            new_population[i] = child1;
            new_population[i + iters] = child2;
            new_fitness[i] = child1_fitness;
            new_fitness[i + iters] = child2_fitness;

            // Update the optimal solution if a new one is found
            #pragma omp critical
            {
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
void GeneticAlgorithmSolver::select_elite(
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

/**
 * @brief Evaluates the given assignment
 * 
 * @param assignment The assignment to be evaluated
 * @param flipped_var The variable that was flipped to obtain the assignment
 * @return int The new number of satisfied clauses of the assignment
 */
int GeneticAlgorithmSolver::eval_function(
    vector<bool> &assignment,
    int flipped_var,
    int current_n_satisfied
) {
    int new_n_satisfied = current_n_satisfied;

    // Scan the clauses affected by the flipped variable
    for (auto i : affected_clauses[flipped_var]) {

        bool already_satisfied = false;
        int flipped_literal = -1;

        for (auto literal : instance.clauses[i]) {
            if (literal>>1 == flipped_var) {
                if (flipped_literal == -1) flipped_literal = literal;
                else if (flipped_literal != literal) {
                    // Edge case: the clause contains p v -p
                    already_satisfied = true;
                    break;
                }
                continue;
            }

            // Check if the clause was already satisfied regardless of the flip
            already_satisfied = instance.is_literal_true(literal, assignment);
            if (already_satisfied) break;
        }

        // If the clause was not already satisfied, check how the flip affects
        if (!already_satisfied) {
            if (instance.is_literal_true(flipped_literal, assignment)) 
                new_n_satisfied++;
            else
                new_n_satisfied--;
        }
    }

    return new_n_satisfied;
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
    vector<bool> &child1,
    vector<bool> &child2
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

void GeneticAlgorithmSolver::two_point_crossover(
    const vector<bool> &parent1,
    const vector<bool> &parent2,
    vector<bool> &child1,
    vector<bool> &child2
) { 
    // Choose two random points to cross
    int cross_point1 = rand() % instance.n_vars;
    int cross_point2 = rand() % instance.n_vars;
    if (cross_point1 > cross_point2) swap(cross_point1, cross_point2);

    // First children gets the first part of the first parent and the second
    // part of the second parent; and viceversa for the second child
    for (int i = 0; i < cross_point1; i++) {
        child1.push_back(parent1[i]);
        child2.push_back(parent2[i]);
    }
    for (int i = cross_point1; i < cross_point2; i++) {
        child1.push_back(parent2[i]);
        child2.push_back(parent1[i]);
    }
    for (int i = cross_point2; i < instance.n_vars; i++) {
        child1.push_back(parent1[i]);
        child2.push_back(parent2[i]);
    }
}

/**
 * @brief Mutate a solution
 *
 * @param solution The solution to mutate
 */
int GeneticAlgorithmSolver::mutate(vector<bool> &solution) {
    if (rand() % 100 < mutation_probability) {
        for (int i = 0; i < instance.n_vars; i++) {
            if (rand() % 100 < mutation_percent)
                solution[i] = !solution[i];
        }
    }

    return compute_n_satisfied(solution);
}


int GeneticAlgorithmSolver::mutateGreedy(vector<bool> &solution) {
    int n_satisfied = compute_n_satisfied(solution);
    if (rand() % 100 < mutation_probability) {
        for (int i = 0; i < instance.n_vars; i++) {
            // Flip a variable and evaluate the new solution
            solution[i] = !solution[i];

            int new_n_satisfied = eval_function(solution, i, n_satisfied);
            if (new_n_satisfied > n_satisfied) {
                n_satisfied = new_n_satisfied;
                break;
            }

            // If the flip didn't improve the solution, flip it back
            solution[i] = !solution[i];
        }
    }
    return n_satisfied;
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
    cout << "c Genetic Algorithm Solver" << endl;
    cout << "c MAX_GENS = " << MAX_GENS << endl;
    cout << "c population_size = " << population_size << endl;
    cout << "c tournament_size = " << tournament_size << endl;
    cout << "c mutation_probability = " << mutation_probability << endl;
    cout << "c mutation_percent = " << mutation_percent << endl;
    cout << "c elite_percent = " << elite_percent << endl;
    cout << "c seed = " << seed << endl;
    MaxSATSolver::print_solution();
}
