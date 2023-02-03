# Compile c++ code
CC = g++
CFLAGS = -Wall -g -std=c++11
BUILD_DIR = build
SRC_DIR = src

all: bin/maxsatsolver

bin/maxsatsolver: src/main.cpp build/SATInstance.o
	$(CC) $(CFLAGS) -o bin/maxsatsolver src/main.cpp build/SATInstance.o

build/SATInstance.o: src/SATInstance.cpp include/SATInstance.hpp
	$(CC) $(CFLAGS) -c src/SATInstance.cpp -o build/SATInstance.o

clean:
	rm -f bin/maxsatsolver build/*.o