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
#define NO_EDGE 0                            // represent no edge between two vertices as infinity

namespace shayg {

// ~~~ helper functions ~~~

bool isSubMatrix(const vector<vector<int>>& subMatrix, const vector<vector<int>>& matrix);
bool matrixEqual(const vector<vector<int>>& mat1, const vector<vector<int>>& mat2);

// ~~~ done helper functions ~~~

/**
 * @brief an abstract class that represents a graph as an adjacency matrix
 */
class Graph {
   private:
    vector<vector<int>> adjMat;
    bool isDirected;
    bool isWeighted;
    bool haveNegativeEdgeWeight;

    /**
     * @brief modify the weights of the edges in the graph using a function
     * @note if func return 0, the edge will be removed.
     * @param func the function that will be applied to the weights of the edges, will take the current weight as an argument and change it.
     */
    void modifyEdgeWeights(function<int(int)> func);

    /**
     * @brief modify the weights of the edges in the graph using a function
     * @note if func return 0, the edge will be removed.
     * @param other the other graph that will be used to modify the weights of the edges
     * @param func the function that will be applied to the weights of the edges, will take the current weight and the weight of the other graph as arguments and change the current weight with the result of the function.
     * @throw invalid_argument if the two graphs have different number of vertices (the adjacency matrices are not the same size)
     * @throw invalid_argument if the two graphs are not the same type (directed/undirected)
     */
    void modifyEdgeWeights(const Graph& other, function<int(int, int)> func);

    /**
     * @brief update the isWeighted and haveNegativeEdgeWeight fields if needed.
     * check if a directed graph is a symmetric matrix
     */
    void updateData();

   public:
    /**
     * @brief Construct a new Graph object
     * @param isDirected whether the graph is directed or not. Default is false.
     */
    Graph(bool isDirected = false);

    /**
     * @brief Load the graph from an adjacency matrix
     * @param adjMat the adjacency matrix
     * @throw invalid_argument if the graph is not a square matrix or the diagonal of the matrix is not NO_EDGE
     * @throw invalid_argument if the graph is undirected and the matrix is not symmetric
     */
    void loadGraph(const vector<vector<int>>& adjMat);

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
    vector<vector<int>>& getGraphRef() { return adjMat; }

    /**
     * @brief get the number of vertices and edges in the graph
     * @return size_t the number of vertices in the graph (|V|)
     */
    size_t getNumVertices() const;

    /**
     * @brief get the number of vertices and edges in the graph
     * @return size_t the number of edges in the graph (|E|)
     */
    size_t getNumEdges() const;

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
     * @brief Unary + operator
     * @return Graph a copy of the current graph
     */
    Graph operator+() {
        return *this;  // return a copy of the current graph
    }

    /**
     * @brief Binary + operator.
     * Will return a new graph that is the sum of the two graphs. (the sum of the adjacency matrices)
     * If A(u, v) + B(u, v) = 0, the edge will be removed.
     * @param other the other graph
     * @return Graph the sum of the two graphs
     * @throw invalid_argument if the two graphs have different number of vertices (the adjacency matrices are not the same size)
     * @throw invalid_argument if the two graphs are not the same type (directed/undirected)
     */
    Graph operator+(const Graph& other) {
        Graph newGraph = *this;  // copy the current graph
        newGraph.modifyEdgeWeights(other, [](int a, int b) { return a + b; });
        return newGraph;
    }
    /**
     * @brief += operator.
     * Will add the other graph to the current graph.
     * If A(u, v) + B(u, v) = 0, the edge will be removed.
     * @param other the other graph
     * @return the current graph after adding the other graph
     * @throw invalid_argument if the two graphs have different number of vertices (the adjacency matrices are not the same size)
     * @throw invalid_argument if the two graphs are not the same type (directed/undirected)
     */
    Graph& operator+=(const Graph& other) {
        *this = *this + other;  // call the + operator and assign the result to the current graph
        return *this;
    }

    /**
     * @brief Unary - operator
     * will return a new graph that is the negative of the current graph. (the negative of the adjacency matrix)
     * @return Graph the negative of the current graph
     */
    Graph operator-() {
        Graph g = *this;
        g.modifyEdgeWeights([](int weight) { return -weight; });
        return g;
    }

    /**
     * @brief Binary - operator
     * Will return a new graph that is the difference of the two graphs. (the difference of the adjacency matrices)
     * If A(u, v) - B(u, v) = 0, the edge will be removed.
     * @param other the other graph
     * @return A new graph that is the difference of the two graphs
     * @throw invalid_argument if the two graphs have different number of vertices (the adjacency matrices are not the same size)
     * @throw invalid_argument if the two graphs are not the same type (directed/undirected)
     */
    Graph operator-(const Graph& other) {
        Graph newGraph = *this;  // copy the current graph
        newGraph.modifyEdgeWeights(other, [](int a, int b) { return a - b; });
        return newGraph;
    }

    /**
     * @brief -= operator
     * Will subtract the other graph from the current graph.
     * If A(u, v) - B(u, v) = 0, the edge will be removed.
     * @param other the other graph
     * @return a reference to the current graph after subtracting the other graph
     * @throw invalid_argument if the two graphs have different number of vertices (the adjacency matrices are not the same size)
     * @throw invalid_argument if the two graphs are not the same type (directed/undirected)
     */
    Graph& operator-=(const Graph& other) {
        *this = *this - other;  // call the - operator and assign the result to the current graph
        return *this;
    }

    /**
     * @brief prefix ++ operator
     * Will increment the weight of all edges by 1.
     * If the weight of an edge is NO_EDGE, it will remain NO_EDGE.
     * If the weight of an edge is -1, then the edge will be removed.
     * @return a reference to the current graph after incrementing the weight of all edges by 1.
     *
     */
    Graph& operator++() {
        modifyEdgeWeights([](int weight) { return weight + 1; });  // call the lambda function with the current weight
        return *this;
    }

    /**
     * @brief postfix ++ operator
     * Will increment the weight of all edges by 1.
     * If the weight of an edge is NO_EDGE, it will remain NO_EDGE.
     * If the weight of an edge is -1, then the edge will be removed.
     */
    Graph operator++(int) {
        Graph g = *this;
        g.modifyEdgeWeights([](int weight) { return weight + 1; });
        return g;
    }

    /**
     * @brief prefix -- operator
     * Will decrement the weight of all edges by 1.
     * If the weight of an edge is NO_EDGE, it will remain NO_EDGE.
     * If the weight of an edge is 1, then the edge will be removed.
     * @return a reference to the current graph after decrementing the weight of all edges by 1.
     *
     */
    Graph& operator--() {
        modifyEdgeWeights([](int weight) { return weight - 1; });  // call the lambda function with the current weight
        return *this;
    }

    /**
     * @brief postfix -- operator
     * Will decrement the weight of all edges by 1.
     * If the weight of an edge is NO_EDGE, it will remain NO_EDGE.
     * If the weight of an edge is 1, then the edge will be removed.
     */
    Graph operator--(int) {
        Graph g = *this;
        g.modifyEdgeWeights([](int weight) { return weight - 1; });
        return g;
    }

    /**
     * @brief Binary * operator
     * Will return a new graph that is the matrix multiplication of the two graphs. (the matrix multiplication of the adjacency matrices)
     * If AB(u, v) = 0, the edge will be removed.
     * @param other the other graph
     * @return a new graph that is the matrix multiplication of the two graphs
     * @throw invalid_argument if the two graphs have different number of vertices (the adjacency matrices are not the same size)
     * @throw invalid_argument if the two graphs are not the same type (directed/undirected)
     */
    Graph operator*(const Graph& other) const {
        if (this->getNumVertices() != other.getNumVertices()) {
            throw std::invalid_argument("The two graphs have different number of vertices.");
        }
        if (this->isDirected != other.isDirected) {
            throw std::invalid_argument("The two graphs are not the same type (directed/undirected).");
        }

        Graph g = *this;

        // do matrix multiplication on the adjacency matrices
        // adjList[i][j] = sum(adjList[i][k] * adjList[k][j]) for all k
        for (size_t i = 0; i < getNumVertices(); i++) {
            for (size_t j = 0; j < adjMat[i].size(); j++) {
                int sum = 0;
                for (size_t k = 0; k < getNumVertices(); k++) {
                    if (adjMat[i][k] != NO_EDGE && other.adjMat[k][j] != NO_EDGE)
                        sum += adjMat[i][k] * other.adjMat[k][j];
                }
                if (sum != 0) {
                    g.adjMat[i][j] = sum;
                } else {
                    g.adjMat[i][j] = NO_EDGE;
                }
            }
        }
        return g;
    }

    /**
     * @brief *= operator
     * Will multiply the current graph by the other graph.
     * If AB(u, v) = 0, the edge will be removed.
     * @param other the other graph
     * @return a reference to the current graph after multiplying it by the other graph
     * @throw invalid_argument if the two graphs have different number of vertices (the adjacency matrices are not the same size)
     * @throw invalid_argument if the two graphs are not the same type (directed/undirected)
     */
    Graph& operator*=(const Graph& other) {
        *this = *this * other;  // call the * operator and assign the result to the current graph
        return *this;
    }

    /**
     * @brief Binary * operator with a scalar
     * Will return a new graph that is the current graph multiplied by a scalar.
     * If A(u, v) * factor = 0, the edge will be removed. (iff factor = 0 || A(u, v) = 0)
     *
     * @param factor the scalar
     * @return a new graph that is the current graph multiplied by a scalar
     */
    Graph operator*(int factor) const {
        Graph g = *this;
        g.modifyEdgeWeights([factor](int weight) { return weight * factor; });
        return g;
    }

    /**
     * @brief *= operator with a scalar
     * Will multiply the current graph by a scalar.
     * If A(u, v) * factor = 0, the edge will be removed. (iff factor = 0 || A(u, v) = 0)
     * @param factor the scalar
     * @return a reference to the current graph after multiplying it by a scalar
     */
    Graph& operator*=(int factor) {
        *this = *this * factor;
        return *this;
    }

    /**
     * @brief Binary / operator with a scalar   
     * Will return a new graph that is the current graph divided by a scalar.
     * If A(u, v) / factor = 0, the edge will be removed. (iff A(u, v) = 0)
     * @param factor the scalar (the divisor)
     * @return a new graph that is the current graph divided by a scalar
     * @throw invalid_argument if the factor is 0
     *  
    */
    Graph operator/(int factor) const {
        if (factor == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        Graph g = *this;
        g.modifyEdgeWeights([factor](int weight) { return weight / factor; });
        return g;
    }

    /**
     * @brief /= operator with a scalar
     * Will divide the current graph by a scalar.
     * If A(u, v) / factor = 0, the edge will be removed. (iff A(u, v) = 0)
     * @param factor the scalar (the divisor)
     * @return a reference to the current graph after dividing it by a scalar
     * @throw invalid_argument if the factor is 0
    */
    Graph& operator/=(int factor) {
        *this = *this / factor;
        return *this;
    }

    // ~~~ Comparison operators ~~~

    /**
     * @brief Overload the == operator
     * A == B if the adjacency matrices of the two graphs are the same.
     * or if not A < B and not B < A
     */
    bool operator==(const Graph& other) const {
        // !(A < B) && !(B < A)) == !(A < B || B < A)
        return !(*this < other || other < *this);
    }

    /**
     * @brief Overload the != operator
     * will return the opposite of the == operator
     * @param other the other graph
     * @return true if the current graph is not equal to the other graph, false otherwise
     */
    bool operator!=(const Graph& other) const {
        return !(*this == other);
    }

    /**
     * @brief Overload the < operator
     * To check if A < B,  we use the adjacency matrices of the two graphs.
     * Graph A is less than graph B if:
     * 1. the adjacency matrix of A is submatrix of the adjacency matrix of B.
     * 2. otherwise, |E(A)| < |E(B)| (number of edges in A is less than the number of edges in B)
     * 3. otherwise, |V(A)| < |V(B)| (number of vertices in A is less than the number of vertices in B)
     *
     * otherwise, A is not less than B.
     *
     * @param other the other graph
     * @return true if the current graph is less than the other graph, false otherwise
     */
    bool operator<(const Graph& other) const {
        // if they both empty graphs (no vertices and edges) return false
        if (this->adjMat.empty() && other.adjMat.empty()) {
            return false;
        }

        // if the current graph is empty and the other graph is not empty, return true (A is submatrix of B)
        if (this->adjMat.empty()) {
            return true;
        }

        // if the other graph is empty and the current graph is not empty, return false
        if (other.adjMat.empty()) {
            return false;
        }

        // if the two graphs have the same adjacency matrix, return false
        if (matrixEqual(this->adjMat, other.adjMat)) {
            return false;
        }

        // check if the adjacency matrix of the current graph is submatrix of the adjacency matrix of the other graph
        if (isSubMatrix(this->adjMat, other.adjMat)) {
            return true;
        }
        // A is not a submatrix of B - check the number of edges

        // compare the number of edges
        size_t edgeA = this->getNumEdges();
        size_t edgeB = other.getNumEdges();

        if (edgeA < edgeB) {
            return true;
        } else if (edgeA > edgeB) {
            return false;
        } else {  // if the number of edges is the same
            return getNumVertices() < other.getNumVertices();
        }
    }

    /**
     * @brief Overload the > operator
     * will return the opposite of the < operator
     * @param other the other graph
     * @return true if the current graph is greater than the other graph, false otherwise
     */
    bool operator>(const Graph& other) const {
        return other < *this;
    }

    /**
     * @brief Overload the <= operator
     * @param other the other graph
     * @return true if the current graph is less than or equal to the other graph, false otherwise
     */
    bool operator<=(const Graph& other) const {
        return *this < other || *this == other;
    }

    /**
     * @brief Overload the >= operator
     * @param other the other graph
     * @return true if the current graph is greater than or equal to the other graph, false otherwise
     */
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

        for (size_t i = 0; i < graph.getNumVertices(); i++) {
            os << i << ": ";
            for (size_t j = 0; j < graph.adjMat[i].size(); j++) {
                if (graph.adjMat[i][j] != NO_EDGE)
                    os << graph.adjMat[i][j] << " ";
                else
                    os << "X ";
            }
            os << std::endl;
        }

        return os;
    }
};

}  // namespace shayg