# @author Shay Gali
# @link shay.gali@msmail.ariel.ac.il

CXX=clang++-9
CXXFLAGS=-std=c++2a -g -Werror -Wsign-conversion
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all --error-exitcode=99
TIDY_FLAGS=-checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --


.PHONY: all clean tidy

all: Graph.o

Graph.o: Graph.cpp Graph.hpp
	$(CXX) $(CXXFLAGS) --compile $< -o $@

tidy:
	clang-tidy Graph.cpp Graph.hpp $(TIDY_FLAGS)

clean:
	rm -f *.o
