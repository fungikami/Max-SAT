# Compile c++ code
CC = g++
CFLAGS = -Wall -g -std=c++11
BUILD_DIR = build
BIN_DIR = bin
SRC_DIR = src
INCLUDE_DIR = include

all: $(BIN_DIR)/maxsatsolver

$(BIN_DIR)/maxsatsolver: $(SRC_DIR)/main.cpp $(BUILD_DIR)/SATInstance.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/maxsatsolver $(SRC_DIR)/main.cpp $(BUILD_DIR)/SATInstance.o

$(BUILD_DIR)/SATInstance.o: $(SRC_DIR)/SATInstance.cpp $(INCLUDE_DIR)/SATInstance.hpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/SATInstance.cpp -o $(BUILD_DIR)/SATInstance.o

clean:
	rm -f $(BIN_DIR)/maxsatsolver $(BUILD_DIR)/*.o

$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR) $(SRC_DIR) $(INCLUDE_DIR))