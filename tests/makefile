# @author Shay Gali
# @link shay.gali@msmail.ariel.ac.il


CXX=clang++-9
CXXFLAGS=-std=c++2a -g -Werror -Wsign-conversion # c++20
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all --error-exitcode=99
TIDY_FLAGS=-checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory,-readability-magic-numbers,-cppcoreguidelines-avoid-magic-numbers,-readability-isolate-declaration --warnings-as-errors=-* --

SOURCES= ../graph/Graph.cpp ../algorithms/Algorithms.cpp
OBJECTS=$(subst .cpp,.o,$(SOURCES)) # replace .cpp with .o in SOURCES
TEST_SOURCES=test.cpp TestCounter.cpp GraphTest.cpp
TEST_OBJECTS=$(subst .cpp,.o,$(TEST_SOURCES))

.PHONY: clean test graph algorithms valgrind tidy

run: test
	./test

test: $(TEST_OBJECTS) graph algorithms
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) $(OBJECTS) -o test

tidy:
	clang-tidy $(TEST_SOURCES) $(TIDY_FLAGS)
	
graph:
	make -C ../graph all

algorithms:
	make -C ../algorithms all

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) --compile $< -o $@

clean:
	make -C ../graph clean
	make -C ../algorithms clean
	rm -f *.o test core