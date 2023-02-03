# Compile c++ code
CC = g++
CFLAGS = -Wall -g -std=c++11

all: maxsatsolver

maxsatsolver: main.cpp SATInstance.o
	$(CC) $(CFLAGS) -o maxsatsolver main.cpp SATInstance.o

SATInstance.o: SATInstance.cpp SATInstance.hpp
	$(CC) $(CFLAGS) -c SATInstance.cpp

clean:
	rm -f maxsatsolver