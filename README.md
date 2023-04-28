# Max-SAT
In computational complexity theory, the maximum satisfiability problem (Max-SAT) is the problem of determining the maximum number of clauses, of a given Boolean formula in conjunctive normal form (CNF), that can be made true by an assignment of truth values to the variables of the formula. It is a generalization of the Boolean satisfiability problem (SAT), which asks whether there exists such an assignment making all clauses true; whereas Max-SAT asks for the largest fraction of clauses that can simultaneously be made true. The decision version of Max-SAT is NP-complete, like SAT.

Different metaheuristics are used to solve the Max-SAT problem in this project. The metaheuristics used are: local search, guided local search, simulated annealing, genetic algorithm, memetic algorithm, ant colony optimization and solution tree. The metaheuristics are implemented in C++. The input file is a CNF formula in DIMACS format. The output is the best solution found by the metaheuristic, the number of satisfied clauses and the execution time. 


## Metaheuristics

### Local Search

Local search (LS) is a heuristic method for solving computationally hard optimization problems. It is a local search algorithm that starts from an initial solution and iteratively moves to a neighboring solution. Each move to a neighboring solution is called a local move. The objective function is improved at each step. The algorithm terminates when no local move can improve the solution. Local search algorithms are widely used in combinatorial optimization.

### Guided Local Search

Guided local search (GLS) is a local search algorithm that iteratively moves from one solution to a better solution. The algorithm is guided by a heuristic function that is constructed from a combination of a heuristic function and a penalty function. The heuristic function is used to guide the search towards better solutions. The penalty function is used to avoid cycling. The algorithm terminates when no local move can improve the solution.

### Simulated Annealing

Simulated annealing (SA) is a probabilistic technique for approximating the global optimum of a given function. Specifically, it is a metaheuristic to approximate global optimization in a large search space for an optimization problem. Simulated annealing interprets slow cooling as a slow decrease in the probability of accepting worse solutions as it explores the solution space. Accepting worse solutions is a fundamental property of metaheuristics because it allows for a more extensive search for the global optimum. This property allows the algorithm to avoid being trapped in local optima, which is a problem for hill climbing algorithms. 

### Genetic Algorithm

A genetic algorithm (GA) is a metaheuristic inspired by the process of natural selection that belongs to the larger class of evolutionary algorithms (EA). Genetic algorithms are commonly used to generate high-quality solutions to optimization and search problems by relying on bio-inspired operators such as mutation, crossover and selection. 

### Memetic Algorithm

A memetic algorithm (MA) is a population-based method in which solutions are also subject to local improvement phases. Memetic algorithms are population-based, stochastic search algorithms for solving optimization problems. They are related to genetic algorithms (GAs) but differ in that solutions in a population are subject to local improvement (in this case, a local search) before being used in genetic operators (crossover and mutation). 

### Ant Colony Optimization

Ant colony optimization (ACO) is a metaheuristic that is inspired by the foraging behavior of ants. In the natural world, ants of some species (initially) wander randomly, and upon finding food return to their colony while laying down pheromone trails. If other ants find such a path, they are likely not to keep travelling at random, but instead to follow the trail, returning and reinforcing it if they eventually find food (see Ant communication). Over time, however, the pheromone trail starts to evaporate, thus reducing its attractive strength. The more time it takes for an ant to travel down the path and back again, the more time the pheromones have to evaporate. A short path, by comparison, gets marched over more frequently, and thus the pheromone density becomes higher on shorter paths than longer ones. Pheromone evaporation also has the advantage of avoiding the convergence to a locally optimal solution. If there were no evaporation at all, the paths chosen by the first ants would tend to be excessively attractive to the following ones. In that case, the exploration of the solution space would be constrained. The influence of pheromone evaporation in real ant systems is unclear, but it is very important in artificial systems. 

### Solution Tree

The solution tree consists of exploring the solution space by building a tree that represents the possible neighborhoods of an initial solution. This process is repeated for a finite number of iterations, taking the best solution found so far as the root solution. The tree is built by applying a neighborhood function to the root solution, generating a set of solutions that are added as children of the root. This process is repeated for each child of the root, generating a new set of solutions that are added as children of the corresponding child. This process is repeated until the tree is completed.


## How to run
```
make
./main.sh <input_file>
```