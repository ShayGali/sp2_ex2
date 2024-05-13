CXX=clang++-9
CXXFLAGS=-std=c++2a -g -Werror -Wsign-conversion
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99

.PHONY: all clean

run: all
	./main

all: Graph.o main

main: main.cpp Graph.o
	$(CXX) $(CXXFLAGS) $^ -o $@

Graph.o: Graph.cpp Graph.hpp
	$(CXX) $(CXXFLAGS) --compile $< -o $@

clean:
	rm -f *.o main
