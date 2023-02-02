# Compile c++ code
CC = g++
CFLAGS = -Wall -g -std=c++11

all:
	$(CC) $(CFLAGS) -o satsolver main.cpp