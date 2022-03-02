CC=g++
CXXFLAGS=-Wall -std=c++14 
DEPENDHPP=$(wildcard ./headers/*.hpp)

SortTest: $(DEPENDHPP)
	$(CC) CXXFLAGS -o SortTest

.PHONY:clean
clean:
	-rm -rf *.o SortTest