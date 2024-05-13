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

    // by default, the graph is not weighted and does not have negative edge weight
    // will be updated in the loadGraph method
    this->isWeighted = false;
    this->haveNegativeEdgeWeight = false;
}

void Graph::loadGraph(const vector<vector<int>>& ajdList) {
    this->isWeighted = false;
    this->haveNegativeEdgeWeight = false;

    this->ajdList = ajdList;
    /*
     * update the isWeighted and haveNegativeEdgeWeight fields.
     * create an deep copy of the adjacency list.
     */

    // check if the graph is a square matrix
    // check if the diagonal of the matrix is 0
    for (size_t i = 0; i < ajdList.size(); i++) {
        if (ajdList.size() != ajdList[i].size()) {
            throw invalid_argument("Invalid graph: The graph is not a square matrix.(" + std::to_string(i) + "th row has " + std::to_string(ajdList[i].size()) + " elements.)");
        }
        if (ajdList[i][i] != NO_EDGE) {
            throw invalid_argument("The diagonal of the matrix must be NO_EDGE. (the " + std::to_string(i) + "th node is not a NO_EDGE)");
        }
    }

    // update the isWeighted and haveNegativeEdgeWeight fields if needed
    // check if a directed graph is a symmetric matrix
    for (size_t i = 0; i < ajdList.size(); i++) {
        for (size_t j = 0; j < ajdList[i].size(); j++) {
            if (ajdList[i][j] != NO_EDGE) {
                if (ajdList[i][j] != 1) {
                    this->isWeighted = true;
                }
                if (ajdList[i][j] < 0) {
                    this->haveNegativeEdgeWeight = true;
                }
            }

            if (!this->isDirected && ajdList[i][j] != ajdList[j][i]) {
                throw invalid_argument("Invalid graph: The graph is not symmetric.(mat[" + std::to_string(i) + "][" + std::to_string(j) + "] = " + std::to_string(ajdList[i][j]) + " and mat[" + std::to_string(j) + "][" + std::to_string(i) + "] = " + std::to_string(ajdList[j][i]) + ")");
            }
        }
    }
}

void Graph::printGraph() const {
    int count_edges = 0;
    for (size_t i = 0; i < ajdList.size(); i++) {
        for (size_t j = 0; j < ajdList[i].size(); j++) {
            if (ajdList[i][j] != NO_EDGE) {
                count_edges++;
            }
        }
    }
    if (this->isDirected) {
        std::cout << "Directed graph with " << ajdList.size() << " vertices and " << count_edges << " edges." << std::endl;
    } else {
        std::cout << "Undirected graph with " << ajdList.size() << " vertices and " << count_edges / 2 << " edges." << std::endl;
    }
}

void Graph::printAdjMat() const {
    this->printGraph();

    for (size_t i = 0; i < ajdList.size(); i++) {
        std::cout << i << ": ";
        for (size_t j = 0; j < ajdList[i].size(); j++) {
            if (ajdList[i][j] != NO_EDGE) {
                std::cout << ajdList[i][j] << " ";
            } else {
                std::cout << "X ";
            }
        }
        std::cout << std::endl;
    }
}

vector<vector<int>> Graph::getGraph() const { return this->ajdList; }

bool Graph::isDirectedGraph() const { return this->isDirected; }
bool Graph::isWeightedGraph() const { return this->isWeighted; }

bool Graph::isHaveNegativeEdgeWeight() const { return this->haveNegativeEdgeWeight; }

size_t Graph::getNumVertices() const {
    return this->ajdList.size();
}
size_t Graph::getNumEdges() const {
    size_t count_edges = 0;
    for (size_t i = 0; i < ajdList.size(); i++) {
        for (size_t j = 0; j < ajdList[i].size(); j++) {
            if (ajdList[i][j] != NO_EDGE) {
                count_edges++;
            }
        }
    }
    return count_edges;
}

void Graph::changeEdgeWeight(size_t u, size_t v, int weight) {
    if (u < 0 || v < 0 || u >= getNumVertices() || v >= getNumVertices()) {
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

void Graph::modifyEdgeWeights(function<int(int)> func) {
    for (size_t u = 0; u < getNumVertices(); u++) {
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

void Graph::modifyEdgeWeights(const Graph& other, function<int(int, int)> func) {
    if (this->getNumVertices() != other.getNumVertices()) {
        throw std::invalid_argument("The two graphs have different number of vertices.");
    }

    if (this->isDirected != other.isDirected) {
        throw std::invalid_argument("The two graphs are not the same type.");
    }

    for (size_t u = 0; u < getNumVertices(); u++) {
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

// ~~~ helper functions ~~~

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