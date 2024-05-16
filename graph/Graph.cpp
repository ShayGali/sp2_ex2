/**
 * @author Shay Gali
 * @link shay.gali@msmail.ariel.ac.il
 */
#include "Graph.hpp"

#include <iostream>
#include <stdexcept>

using namespace shayg;

using std::invalid_argument;

Graph::Graph(bool isDirected) {
    this->isDirected = isDirected;
}

void Graph::loadGraph(const vector<vector<int>>& adjMat) {
    this->adjMat = adjMat;
    /*
     * update the isWeighted and haveNegativeEdgeWeight fields.
     * create an deep copy of the adjacency list.
     */

    // check if the graph is a square matrix
    // check if the diagonal of the matrix is 0
    for (size_t i = 0; i < adjMat.size(); i++) {
        if (adjMat.size() != adjMat[i].size()) {
            throw invalid_argument("Invalid graph: The graph is not a square matrix.(" + std::to_string(i) + "th row has " + std::to_string(adjMat[i].size()) + " elements.)");
        }
        if (adjMat[i][i] != NO_EDGE) {
            throw invalid_argument("The diagonal of the matrix must be NO_EDGE. (the " + std::to_string(i) + "th node is not a NO_EDGE)");
        }
    }

    updateData();
}

void Graph::printGraph(std::ostream& out) const {
    int count_edges = getNumEdges();
    if (this->isDirected) {
        out << "Directed graph with " << adjMat.size() << " vertices and " << count_edges << " edges." << std::endl;
    } else {
        out << "Undirected graph with " << adjMat.size() << " vertices and " << count_edges << " edges." << std::endl;
    }
}

void Graph::printAdjMat(std::ostream& out) const {
    for (size_t i = 0; i < adjMat.size(); i++) {
        // out << i << ": ";
        for (size_t j = 0; j < adjMat[i].size(); j++) {
            if (adjMat[i][j] != NO_EDGE) {
                out << adjMat[i][j] << " ";
            } else {
                out << "X ";
            }
        }
        out << std::endl;
    }
}

size_t Graph::getNumEdges() const {
    size_t count_edges = 0;
    for (size_t i = 0; i < adjMat.size(); i++) {
        for (size_t j = 0; j < adjMat[i].size(); j++) {
            if (adjMat[i][j] != NO_EDGE) {
                count_edges++;
            }
        }
    }

    // if the graph is undirected, the number of edges is half
    if (!this->isDirected) {
        count_edges /= 2;
    }
    return count_edges;
}

void Graph::updateData() {
    this->isWeighted = false;
    this->haveNegativeEdgeWeight = false;

    for (size_t i = 0; i < adjMat.size(); i++) {
        for (size_t j = 0; j < adjMat[i].size(); j++) {
            if (adjMat[i][j] != NO_EDGE) {
                if (adjMat[i][j] != 1) {
                    this->isWeighted = true;
                }

                if (adjMat[i][j] < 0) {
                    this->haveNegativeEdgeWeight = true;
                }

                if (!this->isDirected && adjMat[i][j] != adjMat[j][i]) {
                    throw invalid_argument("Invalid graph: The graph is not symmetric.(mat[" + std::to_string(i) + "][" + std::to_string(j) + "] = " + std::to_string(adjMat[i][j]) + " and mat[" + std::to_string(j) + "][" + std::to_string(i) + "] = " + std::to_string(adjMat[j][i]) + ")");
                }
            }
        }
    }
}

// ~~~ helper functions for the operators ~~~

void Graph::modifyEdgeWeights(function<int(int)> func) {
    this->isWeighted = false;
    this->haveNegativeEdgeWeight = false;

    for (size_t u = 0; u < getNumVertices(); u++) {
        for (size_t v = 0; v < getNumVertices(); v++) {
            if (adjMat[u][v] != NO_EDGE) {
                int res = func(adjMat[u][v]);
                if (res == 0) {
                    adjMat[u][v] = NO_EDGE;
                } else {
                    adjMat[u][v] = res;

                    // update data if needed
                    if (res != 1) {
                        this->isWeighted = true;
                    }
                    if (res < 0) {
                        this->haveNegativeEdgeWeight = true;
                    }
                }
            }
        }
    }
}

void Graph::modifyEdgeWeights(const Graph& other, function<int(int, int)> func) {
    if (this->getNumVertices() != other.getNumVertices()) {
        throw std::invalid_argument("The two graphs have different number of vertices.");
    }

    if (this->isDirected != other.isDirected) {
        throw std::invalid_argument("The two graphs are not the same type (directed/undirected).");
    }

    this->isWeighted = false;
    this->haveNegativeEdgeWeight = false;

    for (size_t u = 0; u < getNumVertices(); u++) {
        for (size_t v = 0; v < getNumVertices(); v++) {
            if (adjMat[u][v] == NO_EDGE && other.adjMat[u][v] == NO_EDGE) {  // if they are both NO_EDGE - the result edge is NO_EDGE
                adjMat[u][v] = NO_EDGE;
            } else if (adjMat[u][v] == NO_EDGE && other.adjMat[u][v] != NO_EDGE) {  // if one of them is NO_EDGE - the result edge is the other one
                adjMat[u][v] = func(0, other.adjMat[u][v]);
            } else if (adjMat[u][v] != NO_EDGE && other.adjMat[u][v] == NO_EDGE) {
                adjMat[u][v] = func(adjMat[u][v], 0);
            } else {  // if they are both not NO_EDGE - the result edge is the result of the operation
                int res = func(adjMat[u][v], other.adjMat[u][v]);
                if (res == 0 || res == NO_EDGE) {  // if the result is 0 or NO_EDGE - the result edge is NO_EDGE
                    adjMat[u][v] = NO_EDGE;
                } else {
                    adjMat[u][v] = res;
                }
            }

            if (adjMat[u][v] != NO_EDGE) {
                if (adjMat[u][v] != 1) {
                    this->isWeighted = true;
                }
                if (adjMat[u][v] < 0) {
                    this->haveNegativeEdgeWeight = true;
                }
            }
        }
    }
}

bool isSubMatrix(const vector<vector<int>>& subMatrix, const vector<vector<int>>& matrix) {
    if (subMatrix.size() > matrix.size()) {
        return false;
    }

    for (size_t i = 0; i <= matrix.size() - subMatrix.size(); i++) {
        for (size_t j = 0; j <= matrix.size() - subMatrix.size(); j++) {
            bool match = true;
            for (size_t k = 0; k < subMatrix.size(); k++) {
                for (size_t l = 0; l < subMatrix.size(); l++) {
                    if (subMatrix[k][l] != matrix[i + k][j + l]) {
                        match = false;
                        break;
                    }
                }
                if (!match) {
                    break;
                }
            }
            if (match) {
                return true;
            }
        }
    }
    return false;
}

bool matrixEqual(const vector<vector<int>>& mat1, const vector<vector<int>>& mat2) {
    if (mat1.size() != mat2.size()) {
        return false;
    }

    for (size_t i = 0; i < mat1.size(); i++) {
        for (size_t j = 0; j < mat1[i].size(); j++) {
            if (mat1[i][j] != mat2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// ~~~~ operators ~~~~

Graph Graph::operator*(const Graph& other) const {
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
            if (adjMat[i][j] == NO_EDGE) {
                continue;
            }
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

bool Graph::operator<(const Graph& other) const {
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

std::ostream& shayg::operator<<(std::ostream& os, const Graph& graph) {  //~~~ the shayg:: is needed because the operator is defined in the shayg namespace ~~~//
    // graph.printGraph(os);
    graph.printAdjMat(os);
    return os;
}