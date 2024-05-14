/**
 * @author Shay Gali
 * @link shay.gali@msmail.ariel.ac.il
 */
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "../algorithms/Algorithms.hpp"
#include "../graph/Graph.hpp"
#include "doctest.h"

using namespace shayg;
using namespace std;

/**
 * helper function to check if the adjacency matrixes are equal
 */
bool checkMatrixes(const vector<vector<int>>& m1, const vector<vector<int>>& m2, vector<vector<int>>& expected, function<int(int, int)> op) {
    for (size_t i = 0; i < m1.size(); i++) {
        for (size_t j = 0; j < m1.size(); j++) {
            if (m1[i][j] == NO_EDGE && m2[i][j] == NO_EDGE) {  // if they are both NO_EDGE - the expected value should be NO_EDGE
                if (expected[i][j] != NO_EDGE) {
                    cout << "NO_EDGE expected[" << i << "][" << j << "] = " << expected[i][j] << " m1[" << i << "][" << j << "] = " << m1[i][j] << " m2[" << i << "][" << j << "] = " << m2[i][j] << "\n";
                    return false;
                }
            } else if (m1[i][j] == NO_EDGE && m2[i][j] != NO_EDGE) {  // if one of them is NO_EDGE - the expected value should be the other one
                if (expected[i][j] != m2[i][j]) {
                    cout << "m2 expected[" << i << "][" << j << "] = " << expected[i][j] << " m1[" << i << "][" << j << "] = " << m1[i][j] << " m2[" << i << "][" << j << "] = " << m2[i][j] << "\n";
                    return false;
                }
            } else if (m1[i][j] != NO_EDGE && m2[i][j] == NO_EDGE) {
                if (expected[i][j] != m1[i][j]) {
                    cout << "m1 expected[" << i << "][" << j << "] = " << expected[i][j] << " m1[" << i << "][" << j << "] = " << m1[i][j] << " m2[" << i << "][" << j << "] = " << m2[i][j] << "\n";
                    return false;
                }
            } else if (m1[i][j] != NO_EDGE && m2[i][j] != NO_EDGE) {  // if they are both not NO_EDGE - the expected value should be the result of the operation
                int res = op(m1[i][j], m2[i][j]);
                if (res == 0 || res == NO_EDGE) {  // if the result is 0 or NO_EDGE - the expected value should be NO_EDGE
                    if (expected[i][j] != NO_EDGE) {
                        cout << "NO_EDGE expected[" << i << "][" << j << "] = " << expected[i][j] << " m1[" << i << "][" << j << "] = " << m1[i][j] << " m2[" << i << "][" << j << "] = " << m2[i][j] << " res = " << res << "\n";
                        return false;
                    }
                } else if (expected[i][j] != res) {  // if the result is not 0 or NO_EDGE - the expected value should be the result of the operation
                    cout << "expected[" << i << "][" << j << "] = " << expected[i][j] << " m1[" << i << "][" << j << "] = " << m1[i][j] << " m2[" << i << "][" << j << "] = " << m2[i][j] << " res = " << res << "\n";
                    return false;
                }
            }
        }
    }
    return true;
}

TEST_CASE("unary +") {
    SUBCASE("undirected graph") {
        Graph g1(false);
        vector<vector<int>> graph = {
            // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        Graph g2 = +g1;
        CHECK(g1.getGraph() == g2.getGraph());          // check if the adjacency matrix have the same values
        CHECK(&g1 != &g2);                              // check if the address is different
        CHECK(&g1.getGraphRef() != &g2.getGraphRef());  // check if the address of the adjacency matrix is different
    }

    SUBCASE("directed graph") {
        Graph g1(true);
        vector<vector<int>> graph = {
            // clang-format off
            {NO_EDGE, 1,       1      },
            {NO_EDGE, NO_EDGE, 2      },
            {NO_EDGE, NO_EDGE, NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        Graph g2 = +g1;
        CHECK(g1.getGraph() == g2.getGraph());          // check if the adjacency matrix have the same values
        CHECK(&g1 != &g2);                              // check if the address is different
        CHECK(&g1.getGraphRef() != &g2.getGraphRef());  // check if the address of the adjacency matrix is different
    }
}

TEST_CASE("unary -") {
    SUBCASE("undirected graph") {
        Graph g1(false);
        vector<vector<int>> graph = {
            // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
            // clang-format on
        };

        g1.loadGraph(graph);

        Graph g2 = -g1;
        for (size_t i = 0; i < graph.size(); i++) {
            for (size_t j = 0; j < graph[i].size(); j++) {
                if (graph[i][j] == NO_EDGE) {
                    CHECK(g2.getGraph()[i][j] == NO_EDGE);  // check if the edge is NO_EDGE
                } else {
                    CHECK(g2.getGraph()[i][j] == -graph[i][j]);  // check if the edge is the negative of the original edge
                }
            }
        }
        CHECK(&g1 != &g2);                             // check if the address is different
        CHECK(g2.isHaveNegativeEdgeWeight() == true);  // check if the graph have negative edge weight
        CHECK(g2.isWeightedGraph() == true);           // check if the graph is weighted
        CHECK(g2.isDirectedGraph() == false);          // check if the graph is directed
    }
}

TEST_CASE("Binary +") {
    function<int(int, int)> op = [](int a, int b) { return a + b; };
    SUBCASE("undirected graph") {
        Graph g1(false), g2(false), g3(false);
        vector<vector<int>> graph1, graph2, graph3;
        bool matrixCheckResult;
        SUBCASE("Test case 1 - add two graphs") {
            graph1 = {
                // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
                // clang-format on
            };

            graph2 = {
                // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
                // clang-format on
            };

            g1.loadGraph(graph1);
            g2.loadGraph(graph2);

            g3 = g1 + g2;
            graph3 = g3.getGraph();
            matrixCheckResult = checkMatrixes(graph1, graph2, graph3, op);
            CHECK(matrixCheckResult == true);  // check if the adjacency matrix have the correct values
            CHECK(&g1 != &g3);                 // check if the address is different
            CHECK(&g2 != &g3);                 // check if the address is different
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Test case 2 - remove a edges from the graph
        SUBCASE("Test case 2 - add will remove a edges from the graph") {
            graph1 = {
                // clang-format off
            {NO_EDGE, -1,       1      },
            {-1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
                // clang-format on
            };
            g1.loadGraph(graph1);

            graph2 = {
                // clang-format off
            {NO_EDGE, 1,       -1      },
            {1,       NO_EDGE, 1      },
            {-1,       1,       NO_EDGE}
                // clang-format on
            };
            g2.loadGraph(graph2);

            g3 = g1 + g2;
            graph3 = g3.getGraph();
            matrixCheckResult = checkMatrixes(graph1, graph2, graph3, op);

            CHECK(matrixCheckResult == true);               // check if the adjacency matrix have the correct values
            CHECK(g3.isHaveNegativeEdgeWeight() == false);  // check if the graph have negative edge weight
            CHECK(g3.isWeightedGraph() == true);            // check if the graph is weighted
            CHECK(g3.isDirectedGraph() == false);           // check if the graph is directed
            CHECK(g3.getNumEdges() == 1);                   // check if the number of edges is correct
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        SUBCASE("Test case 3 - add a negative edge to the graph") {
            graph1 = {
                // clang-format off
            {NO_EDGE, NO_EDGE, NO_EDGE},
            {NO_EDGE, NO_EDGE, NO_EDGE},
            {NO_EDGE, NO_EDGE, NO_EDGE}
                // clang-format on
            };
            g1.loadGraph(graph1);

            graph2 = {
                // clang-format off
            {NO_EDGE, -1,       NO_EDGE},
            {-1,       NO_EDGE, NO_EDGE},
            {NO_EDGE, NO_EDGE, NO_EDGE}
                // clang-format on
            };
            g2.loadGraph(graph2);
            g3 = g1 + g2;

            graph3 = g3.getGraph();
            matrixCheckResult = checkMatrixes(graph1, graph2, graph3, op);

            CHECK(matrixCheckResult == true);              // check if the adjacency matrix have the correct values
            CHECK(g3.isHaveNegativeEdgeWeight() == true);  // check if the graph have negative edge weight
            CHECK(g3.isWeightedGraph() == true);           // check if the graph is weighted
            CHECK(g3.isDirectedGraph() == false);          // check if the graph is directed
            CHECK(g3.getNumEdges() == 1);                  // check if the number of edges is correct
        }
    }

    SUBCASE("directed graph") {
        Graph g1(true), g2(true), g3(true);
        vector<vector<int>> graph1, graph2, graph3;
        bool matrixCheckResult;
        graph1 = {
            // clang-format off
            {NO_EDGE, 1,       1      },
            {NO_EDGE, NO_EDGE, 2      },
            {NO_EDGE, NO_EDGE, NO_EDGE}
            // clang-format on
        };

        graph2 = {
            // clang-format off
            {NO_EDGE, NO_EDGE, 1      },
            {1,       NO_EDGE, NO_EDGE},
            {NO_EDGE, 1,       NO_EDGE}
            // clang-format on
        };

        g1.loadGraph(graph1);
        g2.loadGraph(graph2);

        g3 = g1 + g2;
        graph3 = g3.getGraph();
        matrixCheckResult = checkMatrixes(graph1, graph2, graph3, op);
        CHECK(matrixCheckResult == true);  // check if the adjacency matrix have the correct values
        CHECK(g3.getNumEdges() == 5);
    }
}

TEST_CASE("Binary -") {
    SUBCASE("undirected graph") {
        function<int(int, int)> op = [](int a, int b) { return a - b; };
        Graph g1(false), g2(false), g3(false);
        vector<vector<int>> graph1, graph2, graph3;
        bool matrixCheckResult;

        SUBCASE("Test case 1 - simple case") {
            graph1 = {
                // clang-format off
            {NO_EDGE, 2,       2      },
            {2,       NO_EDGE, 2      },
            {2,       2,       NO_EDGE}
                // clang-format on
            };

            graph2 = {
                // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
                // clang-format on
            };

            g1.loadGraph(graph1);
            g2.loadGraph(graph2);

            g3 = g1 - g2;
            graph3 = g3.getGraph();
            matrixCheckResult = checkMatrixes(graph1, graph2, graph3, op);
            CHECK(matrixCheckResult == true);  // check if the adjacency matrix have the correct values
            CHECK(&g1 != &g3);                 // check if the address is different
            CHECK(&g2 != &g3);                 // check if the address is different
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Test case 2 - remove a edges from the graph
        SUBCASE("Test case 2 - remove a edges from the graph") {
            graph1 = {
                // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
                // clang-format on
            };
            g1.loadGraph(graph1);

            graph2 = {
                // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
                // clang-format on
            };
            g2.loadGraph(graph2);

            g3 = g1 - g2;
            graph3 = g3.getGraph();
            matrixCheckResult = checkMatrixes(graph1, graph2, graph3, op);

            CHECK(matrixCheckResult == true);               // check if the adjacency matrix have the correct values
            CHECK(g3.isHaveNegativeEdgeWeight() == false);  // check if the graph have negative edge weight
            CHECK(g3.isWeightedGraph() == false);           // check if the graph is weighted
            CHECK(g3.isDirectedGraph() == false);           // check if the graph is directed
            CHECK(g3.getNumEdges() == 0);                   // check if the number of edges is correct
        }
    }
}
