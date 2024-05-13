/**
 * @author Shay Gali
 * @link shay.gali@msmail.ariel.ac.il
 */
#pragma once

#include <functional>
#include <iostream>
#include <limits>
#include <vector>

using std::function;
using std::vector;

#define INF std::numeric_limits<int>::max()  // represent infinity
#define NO_EDGE INF                          // represent no edge between two vertices as infinity

namespace shayg {

/**
 * @brief an abstract class that represents a graph as an adjacency matrix
 */
class Graph {
   private:
    vector<vector<int>> ajdList;
    bool isDirected;
    bool isWeighted;
    bool haveNegativeEdgeWeight;

    /**
     * @brief change the weight of an edge between two vertices.
     * @note this function dont care about symmetry of the matrix of a undirected graph.
     * @param u the first vertex
     * @param v the second vertex
     * @param weight the new weight of the edge
     * @throw invalid_argument if the vertices are invalid
     */
    void changeEdgeWeight(size_t u, size_t v, int weight) {
        if (u < 0 || v < 0 || u >= ajdList.size() || v >= ajdList.size()) {
            throw std::invalid_argument("Invalid vertices.");
        }
        if (weight < 0) {
            this->haveNegativeEdgeWeight = true;
        }
        if (weight != 1) {
            this->isWeighted = true;
        }
        ajdList[u][v] = weight;
    }

    /**
     * @brief modify the weights of the edges in the graph using a function
     * @note if func return 0, the edge will be removed.
     * @param func the function that will be applied to the weights of the edges, will take the current weight as an argument and change it.
     */
    void modifyEdgeWeights(function<int(int)> func) {
        for (size_t u = 0; u < ajdList.size(); u++) {
            for (size_t v = 0; v < ajdList[u].size(); v++) {
                if (ajdList[u][v] != NO_EDGE) {
                    int res = func(ajdList[u][v]);
                    if (res == 0) {
                        changeEdgeWeight(u, v, NO_EDGE);
                    } else {
                        changeEdgeWeight(u, v, res);
                    }
                }
            }
        }
    }

    /**
     * @brief modify the weights of the edges in the graph using a function
     * @note if func return 0, the edge will be removed.
     * @param other the other graph that will be used to modify the weights of the edges
     * @param func the function that will be applied to the weights of the edges, will take the current weight and the weight of the other graph as arguments and change the current weight with the result of the function.
     * @throw invalid_argument if the two graphs have different number of vertices (the adjacency matrices are not the same size)
     * @throw invalid_argument if the two graphs are not the same type (directed/undirected)
     */
    void modifyEdgeWeights(const Graph& other, function<int(int, int)> func) {
        if (this->ajdList.size() != other.ajdList.size()) {
            throw std::invalid_argument("The two graphs have different number of vertices.");
        }

        if (this->isDirected != other.isDirected) {
            throw std::invalid_argument("The two graphs are not the same type.");
        }

        for (size_t u = 0; u < ajdList.size(); u++) {
            for (size_t v = 0; v < ajdList[u].size(); v++) {
                if (ajdList[u][v] != NO_EDGE) {
                    int res = func(ajdList[u][v], other.ajdList[u][v]);
                    if (res == 0) {
                        changeEdgeWeight(u, v, NO_EDGE);
                    } else {
                        changeEdgeWeight(u, v, res);
                    }
                }
            }
        }
    }

   public:
    /**
     * @brief Construct a new Graph object
     * @param isDirected whether the graph is directed or not. Default is false.
     */
    Graph(bool isDirected = false);

    /**
     * @brief Load the graph from an adjacency matrix
     * @param ajdList the adjacency matrix
     * @throw invalid_argument if the graph is not a square matrix or the diagonal of the matrix is not NO_EDGE
     * @throw invalid_argument if the graph is undirected and the matrix is not symmetric
     */
    void loadGraph(const vector<vector<int>>& ajdList);

    /**
     * @brief Print the graph as an adjacency matrix
     * the output will be like this:
     * "Directed/Undirected graph with |V| vertices and |E| edges."
     * where |V| is the number of vertices and |E| is the number of edges.
     */
    void printGraph() const;

    void printAdjMat() const;

    /**
     * @brief return deep copy of the adjacency matrix
     * @return vector<vector<int>> the adjacency matrix
     */
    vector<vector<int>> getGraph() const;

    /**
     * @brief return reference to the adjacency matrix
     * @return vector<vector<int>>& the adjacency matrix
     * @note if you change the returned matrix, the original matrix will be changed too.
     */
    vector<vector<int>>& getGraphRef() {
        return ajdList;
    }

    /**
     * @brief check if the graph is directed
     * @return true if the graph is directed, false otherwise
     */
    bool isDirectedGraph() const;

    /**
     * @brief check if the graph is weighted
     * @return true if the graph is weighted, false otherwise
     */
    bool isWeightedGraph() const;

    /**
     * @brief check if the graph has negative edge weight
     * @return true if the graph has negative edge weight, false otherwise
     */
    bool isHaveNegativeEdgeWeight() const;

    // ~~~ Operators overloading ~~~
    /**
     * Unary + operator
     */
    Graph operator+() {
        return *this;  // return a copy of the current graph
    }

    /**
     * Binary + operator.
     * Will return a new graph that is the sum of the two graphs. (the sum of the adjacency matrices)
     * @param other the other graph
     * @return Graph the sum of the two graphs
     * @throw invalid_argument if the two graphs have different number of vertices (the adjacency matrices are not the same size)
     */
    Graph operator+(const Graph& other) {
        Graph newGraph = *this;  // copy the current graph
        newGraph.modifyEdgeWeights(other, [](int a, int b) { return a + b; });
        return newGraph;
    }
    /**
     * += operator.
     * Will add the other graph to the current graph.
     * @param other the other graph
     * @return the current graph after adding the other graph
     */
    Graph& operator+=(const Graph& other) {
        *this = *this + other;  // call the + operator and assign the result to the current graph
        return *this;
    }

    /**
     * Unary - operator
     * will return a new graph that is the negative of the current graph. (the negative of the adjacency matrix)
     * @return Graph the negative of the current graph
     */
    Graph operator-() {
        Graph g = *this;
        g.modifyEdgeWeights([](int weight) { return -weight; });
        return g;
    }
    /**
     * Binary - operator
     */
    Graph operator-(const Graph& other) {
        if (this->ajdList.size() != other.ajdList.size()) {
            throw std::invalid_argument("The two graphs have different number of vertices.");
        }
        Graph newGraph = *this;  // copy the current graph
        newGraph.modifyEdgeWeights(other, [](int a, int b) { return a - b; });
        return newGraph;
    }

    /**
     * -= operator
     */
    Graph& operator-=(const Graph& other) {
        *this = *this - other;  // call the - operator and assign the result to the current graph
        return *this;
    }

    // Overload the prefix ++ operator
    Graph& operator++() {
        modifyEdgeWeights([](int weight) { return weight + 1; });  // call the lambda function with the current weight
        return *this;
    }

    // Overload the postfix ++ operator
    Graph operator++(int) {
        Graph g = *this;
        g.modifyEdgeWeights([](int weight) { return weight + 1; });
        return g;
    }

    // Overload the prefix -- operator
    Graph& operator--() {
        modifyEdgeWeights([](int weight) { return weight - 1; });  // call the lambda function with the current weight
        return *this;
    }

    // Overload the postfix -- operator
    Graph operator--(int) {
        Graph g = *this;
        g.modifyEdgeWeights([](int weight) { return weight - 1; });
        return g;
    }

    // Overload the * operator
    Graph operator*(const Graph& other) const {
        if (this->ajdList.size() != other.ajdList.size()) {
            throw std::invalid_argument("The two graphs have different number of vertices.");
        }

        Graph g = *this;

        // do matrix multiplication on the adjacency matrices
        // adjList[i][j] = sum(adjList[i][k] * adjList[k][j]) for all k
        for (size_t i = 0; i < ajdList.size(); i++) {
            for (size_t j = 0; j < ajdList[i].size(); j++) {
                int sum = 0;
                for (size_t k = 0; k < ajdList.size(); k++) {
                    if (ajdList[i][k] != NO_EDGE && other.ajdList[k][j] != NO_EDGE)
                        sum += ajdList[i][k] * other.ajdList[k][j];
                }
                if (sum != 0) {
                    g.changeEdgeWeight(i, j, sum);
                } else {
                    g.changeEdgeWeight(i, j, NO_EDGE);
                }
            }
        }
        return g;
    }

    // Overload the *= operator
    Graph& operator*=(const Graph& other) {
        *this = *this * other;  // call the * operator and assign the result to the current graph
        return *this;
    }

    Graph operator*(int factor) const {
        Graph g = *this;
        g.modifyEdgeWeights([factor](int weight) { return weight * factor; });
        return g;
    }

    Graph& operator*=(int factor) {
        *this = *this * factor;
        return *this;
    }

    Graph operator/(int factor) const {
        if (factor == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        Graph g = *this;
        g.modifyEdgeWeights([factor](int weight) { return weight / factor; });
        return g;
    }

    Graph& operator/=(int factor) {
        *this = *this / factor;
        return *this;
    }

    // ~~~ Comparison operators ~~~

    // Overload the == operator
    bool operator==(const Graph& other) const {
        // check if the two graphs have the same number of vertices
        if (this->ajdList.size() != other.ajdList.size()) {
            return false;
        }

        // check if the two graphs have the same adjacency matrix
        for (size_t i = 0; i < ajdList.size(); i++) {
            for (size_t j = 0; j < ajdList[i].size(); j++) {
                if (ajdList[i][j] != other.ajdList[i][j]) {
                    return false;
                }
            }
        }

        throw std::runtime_error("The operator Binary == is not implemented yet.");
        return false;
    }

    // Overload the != operator
    bool operator!=(const Graph& other) const {
        return !(*this == other);
    }

    // Overload the < operator
    bool operator<(const Graph& other) const {
        std::cout << "Binary < operator" << std::endl;
        throw std::runtime_error("The operator Binary < is not implemented yet.");
        return false;
    }

    // Overload the > operator
    bool operator>(const Graph& other) const {
        return other < *this;
    }

    // Overload the <= operator
    bool operator<=(const Graph& other) const {
        return *this < other || *this == other;
    }

    // Overload the >= operator
    bool operator>=(const Graph& other) const {
        return *this > other || *this == other;
    }

    /**
     * @brief Overload the << operator, will print the graph as an adjacency matrix
     * @param os the output stream
     * @param graph the graph
     * @return std::ostream& the output stream
     *
     */
    friend std::ostream& operator<<(std::ostream& os, const Graph& graph) {
        graph.printGraph();

        for (size_t i = 0; i < graph.ajdList.size(); i++) {
            os << i << ": ";
            for (size_t j = 0; j < graph.ajdList[i].size(); j++) {
                if (graph.ajdList[i][j] != NO_EDGE)
                    os << graph.ajdList[i][j] << " ";
                else
                    os << "X ";
            }
            os << std::endl;
        }

        return os;
    }
};

}  // namespace shayg