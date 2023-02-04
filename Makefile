# Compile c++ code
CC = g++
CFLAGS = -Wall -g -std=c++11
BUILD_DIR = build
BIN_DIR = bin
SRC_DIR = src
INCLUDE_DIR = include

all: $(BIN_DIR)/maxsatsolver

$(BIN_DIR)/maxsatsolver: $(SRC_DIR)/main.cpp $(BUILD_DIR)/SATInstance.o $(BUILD_DIR)/BruteForceSolver.o $(BUILD_DIR)/SATSolver.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/maxsatsolver $(SRC_DIR)/main.cpp $(BUILD_DIR)/SATInstance.o $(BUILD_DIR)/BruteForceSolver.o $(BUILD_DIR)/SATSolver.o

$(BUILD_DIR)/BruteForceSolver.o: $(SRC_DIR)/BruteForceSolver.cpp $(INCLUDE_DIR)/BruteForceSolver.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/BruteForceSolver.cpp -o $(BUILD_DIR)/BruteForceSolver.o

$(BUILD_DIR)/SATSolver.o: $(SRC_DIR)/SATSolver.cpp $(INCLUDE_DIR)/SATSolver.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/SATSolver.cpp -o $(BUILD_DIR)/SATSolver.o

$(BUILD_DIR)/SATInstance.o: $(SRC_DIR)/SATInstance.cpp $(INCLUDE_DIR)/SATInstance.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/SATInstance.cpp -o $(BUILD_DIR)/SATInstance.o

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR) $(SRC_DIR) $(INCLUDE_DIR))