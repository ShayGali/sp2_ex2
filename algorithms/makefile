# @author Shay Gali
# @link shay.gali@msmail.ariel.ac.il

CXX=clang++-9
CXXFLAGS=-std=c++2a -g -Werror -Wsign-conversion # c++20
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99

.PHONY: all clean tidy

all: Algorithms.o

Algorithms.o: Algorithms.cpp Algorithms.hpp ../graph/Graph.hpp
	make -C ../graph all 
	$(CXX) $(CXXFLAGS) --compile $< -o $@

tidy:
	clang-tidy Algorithms.cpp Algorithms.hpp -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=-* --


clean:
	rm -f *.o
