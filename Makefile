# Compile c++ code
CC = g++
# Optimize the maximum
CFLAGS = -Wall -std=c++17 -Ofast -march=native -mtune=native -g
BUILD_DIR = build
BIN_DIR = bin
SRC_DIR = src
INCLUDE_DIR = include

all: $(BIN_DIR)/maxsatsolver $(BIN_DIR)/maxsatsolver_test

cmain: $(BIN_DIR)/maxsatsolver

cbenchmark: $(BIN_DIR)/maxsatsolver_test

$(BIN_DIR)/maxsatsolver: $(SRC_DIR)/main.cpp $(BUILD_DIR)/SATInstance.o $(BUILD_DIR)/BruteForceSolver.o $(BUILD_DIR)/LocalSearchSolver.o $(BUILD_DIR)/GLSSolver.o $(BUILD_DIR)/SimulatedAnnealingSolver.o $(BUILD_DIR)/GeneticAlgorithmSolver.o $(BUILD_DIR)/MaxSATSolver.o 
	$(CC) $(CFLAGS) -o $(BIN_DIR)/maxsatsolver $(SRC_DIR)/main.cpp $(BUILD_DIR)/SATInstance.o $(BUILD_DIR)/BruteForceSolver.o $(BUILD_DIR)/LocalSearchSolver.o $(BUILD_DIR)/GLSSolver.o $(BUILD_DIR)/SimulatedAnnealingSolver.o $(BUILD_DIR)/GeneticAlgorithmSolver.o $(BUILD_DIR)/MaxSATSolver.o

$(BIN_DIR)/maxsatsolver_test: $(SRC_DIR)/benchmark.cpp $(BUILD_DIR)/SATInstance.o $(BUILD_DIR)/BruteForceSolver.o $(BUILD_DIR)/LocalSearchSolver.o $(BUILD_DIR)/GLSSolver.o $(BUILD_DIR)/SimulatedAnnealingSolver.o $(BUILD_DIR)/GeneticAlgorithmSolver.o $(BUILD_DIR)/MaxSATSolver.o 
	$(CC) $(CFLAGS) -o $(BIN_DIR)/maxsatsolver_test $(SRC_DIR)/benchmark.cpp $(BUILD_DIR)/SATInstance.o $(BUILD_DIR)/BruteForceSolver.o $(BUILD_DIR)/LocalSearchSolver.o $(BUILD_DIR)/GLSSolver.o $(BUILD_DIR)/SimulatedAnnealingSolver.o $(BUILD_DIR)/GeneticAlgorithmSolver.o $(BUILD_DIR)/MaxSATSolver.o

$(BUILD_DIR)/BruteForceSolver.o: $(SRC_DIR)/BruteForceSolver.cpp $(INCLUDE_DIR)/BruteForceSolver.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/BruteForceSolver.cpp -o $(BUILD_DIR)/BruteForceSolver.o

$(BUILD_DIR)/LocalSearchSolver.o: $(SRC_DIR)/LocalSearchSolver.cpp $(INCLUDE_DIR)/LocalSearchSolver.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/LocalSearchSolver.cpp -o $(BUILD_DIR)/LocalSearchSolver.o

$(BUILD_DIR)/GLSSolver.o: $(SRC_DIR)/GLSSolver.cpp $(INCLUDE_DIR)/GLSSolver.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/GLSSolver.cpp -o $(BUILD_DIR)/GLSSolver.o

$(BUILD_DIR)/GeneticAlgorithmSolver.o: $(SRC_DIR)/GeneticAlgorithmSolver.cpp $(INCLUDE_DIR)/GeneticAlgorithmSolver.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/GeneticAlgorithmSolver.cpp -o $(BUILD_DIR)/GeneticAlgorithmSolver.o

$(BUILD_DIR)/SimulatedAnnealingSolver.o: $(SRC_DIR)/SimulatedAnnealingSolver.cpp $(INCLUDE_DIR)/SimulatedAnnealingSolver.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/SimulatedAnnealingSolver.cpp -o $(BUILD_DIR)/SimulatedAnnealingSolver.o

$(BUILD_DIR)/MaxSATSolver.o: $(SRC_DIR)/MaxSATSolver.cpp $(INCLUDE_DIR)/MaxSATSolver.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/MaxSATSolver.cpp -o $(BUILD_DIR)/MaxSATSolver.o

$(BUILD_DIR)/SATInstance.o: $(SRC_DIR)/SATInstance.cpp $(INCLUDE_DIR)/SATInstance.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/SATInstance.cpp -o $(BUILD_DIR)/SATInstance.o

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR) $(SRC_DIR) $(INCLUDE_DIR))
