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
bool checkMatrixes(const vector<vector<int>>& m1, const vector<vector<int>>& m2, vector<vector<int>>& expected, const function<int(int, int)>& op) {
    for (size_t i = 0; i < m1.size(); i++) {
        for (size_t j = 0; j < m1.size(); j++) {
            if (m1[i][j] == NO_EDGE && m2[i][j] == NO_EDGE) {  // if they are both NO_EDGE - the expected value should be NO_EDGE
                if (expected[i][j] != NO_EDGE) {
                    cout << "NO_EDGE expected[" << i << "][" << j << "] = " << expected[i][j] << " m1[" << i << "][" << j << "] = " << m1[i][j] << " m2[" << i << "][" << j << "] = " << m2[i][j] << "\n";
                    return false;
                }
            } else if (m1[i][j] == NO_EDGE && m2[i][j] != NO_EDGE) {  // if one of them is NO_EDGE - the expected value should be the other one
                if (expected[i][j] != op(0, m2[i][j])) {
                    cout << "the result should be m2[" << i << "][" << j << "] = " << m2[i][j] << " but the result is " << expected[i][j] << "\n";
                    return false;
                }
            } else if (m1[i][j] != NO_EDGE && m2[i][j] == NO_EDGE) {
                if (expected[i][j] != op(m1[i][j], 0)) {
                    cout << "the result should be m1[" << i << "][" << j << "] = " << m1[i][j] << " but the result is " << expected[i][j] << "\n";
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
        Graph g1;
        vector<vector<int>> graph = {
            // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        Graph g2 = +g1;
        CHECK(g1.getGraph() == g2.getGraph());    // check if the adjacency matrix have the same values
        CHECK(&g1 != &g2);                        // check if the address is different
        CHECK(&g1.getGraph() != &g2.getGraph());  // check if the address of the adjacency matrix is different
        CHECK(g2.isDirectedGraph() == false);     // check if the graph is directed
    }

    SUBCASE("directed graph") {
        Graph g1;
        vector<vector<int>> graph = {
            // clang-format off
            {NO_EDGE, 1,       1      },
            {NO_EDGE, NO_EDGE, 2      },
            {NO_EDGE, NO_EDGE, NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        Graph g2 = +g1;
        CHECK(g1.getGraph() == g2.getGraph());    // check if the adjacency matrix have the same values
        CHECK(&g1 != &g2);                        // check if the address is different
        CHECK(&g1.getGraph() != &g2.getGraph());  // check if the address of the adjacency matrix is different
    }
}

TEST_CASE("unary -") {
    SUBCASE("undirected graph") {
        Graph g1;
        vector<vector<int>> graph = {
            // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
            // clang-format on
        };

        g1.loadGraph(graph);

        Graph g2 = -g1;
        vector<vector<int>> expected = {
            // clang-format off
            {NO_EDGE, -1,       -1      },
            {-1,       NO_EDGE, -1      },
            {-1,       -1,       NO_EDGE}
            // clang-format on
        };
        CHECK(g2.getGraph() == expected);              // check if the adjacency matrix have the correct values
        CHECK(&g1 != &g2);                             // check if the address is different
        CHECK(g2.isHaveNegativeEdgeWeight() == true);  // check if the graph have negative edge weight
        CHECK(g2.isWeightedGraph() == true);           // check if the graph is weighted
        CHECK(g2.isDirectedGraph() == false);          // check if the graph is directed
    }
}

TEST_CASE("Binary +") {
    function<int(int, int)> op = [](int a, int b) { return a + b; };
    Graph g1, g2, g3;
    vector<vector<int>> graph1, graph2, graph3;
    bool matrixCheckResult = false;

    SUBCASE("undirected graph") {
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
            {NO_EDGE, -1,      NO_EDGE},
            {-1,      NO_EDGE, NO_EDGE},
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

    SUBCASE("Dir + Undir") {
        graph1 = {
            // clang-format off
            {NO_EDGE, 1,       1      },
            {NO_EDGE, NO_EDGE, 2      },
            {NO_EDGE, NO_EDGE, NO_EDGE}
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
        CHECK(g3.getNumEdges() == 6);
        CHECK(g3.isDirectedGraph() == true);
    }
}

TEST_CASE("Binary -") {
    function<int(int, int)> op = [](int a, int b) { return a - b; };
    Graph g1, g2, g3;
    vector<vector<int>> graph1, graph2, graph3;
    bool matrixCheckResult = false;
    SUBCASE("undirected graph") {
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

    SUBCASE("directed graph") {
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

        // expected result
        graph3 = {
            // clang-format off
            {NO_EDGE, 1,       NO_EDGE},
            {-1     , NO_EDGE, 2      },
            {NO_EDGE, -1,      NO_EDGE}
            // clang-format on
        };

        g1.loadGraph(graph1);
        g2.loadGraph(graph2);

        g3 = g1 - g2;
        CHECK(g3.getGraph() == graph3);  // check if the adjacency matrix have the correct values
        // graph3 = g3.getGraph();
        // matrixCheckResult = checkMatrixes(graph1, graph2, graph3, op);
        // CHECK(matrixCheckResult == true);  // check if the adjacency matrix have the correct values
        CHECK(g3.getNumEdges() == 4);
    }
}

TEST_CASE("+=") {
    function<int(int, int)> op = [](int a, int b) { return a + b; };
    Graph g1, g2;
    vector<vector<int>> graph1, graph2;
    bool matrixCheckResult = false;
    SUBCASE("undirected graph") {
        SUBCASE("Test case 1 - simple case") {
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

            g1 += g2;
            matrixCheckResult = checkMatrixes(graph1, graph2, g1.getGraph(), op);
            CHECK(matrixCheckResult == true);  // check if the adjacency matrix have the correct values
            CHECK(g1.isDirectedGraph() == false);
            CHECK(g1.getNumEdges() == 3);
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
            {NO_EDGE, 1,       -1     },
            {1,       NO_EDGE, 1      },
            {-1,       1,      NO_EDGE}
                // clang-format on
            };
            g2.loadGraph(graph2);

            g1 += g2;
            matrixCheckResult = checkMatrixes(graph1, graph2, g1.getGraph(), op);

            CHECK(matrixCheckResult == true);               // check if the adjacency matrix have the correct values
            CHECK(g1.isHaveNegativeEdgeWeight() == false);  // check if the graph have negative edge weight
            CHECK(g1.isWeightedGraph() == true);            // check if the graph is weighted
            CHECK(g1.isDirectedGraph() == false);           // check if the graph is directed
            CHECK(g1.getNumEdges() == 2);                   // check if the number of edges is correct
        }
        SUBCASE("directed graph") {
            Graph g1, g2;
            vector<vector<int>> graph1, graph2;
            bool matrixCheckResult = false;

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

            g1 += g2;
            matrixCheckResult = checkMatrixes(graph1, graph2, g1.getGraph(), op);
            CHECK(matrixCheckResult == true);  // check if the adjacency matrix have the correct values
            CHECK(g1.getNumEdges() == 5);

            SUBCASE("udir+udir = dir") {
                Graph g1, g2;
                vector<vector<int>> graph1, graph2;
                bool matrixCheckResult = false;

                graph1 = {
                    // clang-format off
                {NO_EDGE, 1,       1      },
                {NO_EDGE, NO_EDGE, 1      },
                {NO_EDGE, NO_EDGE, NO_EDGE}
                    // clang-format on
                };

                graph2 = {
                    // clang-format off
                    {NO_EDGE, NO_EDGE, NO_EDGE},
                    {1,       NO_EDGE, NO_EDGE},
                    {1,       1,       NO_EDGE}
                    // clang-format on
                };

                g1.loadGraph(graph1);
                g2.loadGraph(graph2);

                g1 += g2;

                matrixCheckResult = checkMatrixes(graph1, graph2, g1.getGraph(), op);
                CHECK(matrixCheckResult == true);  // check if the adjacency matrix have the correct values
                CHECK(g1.getNumEdges() == 3);
                CHECK(g1.isDirectedGraph() == false);
                CHECK(g1.isWeightedGraph() == false);
            }
        }

        SUBCASE("Dir + Undir") {
            Graph g1, g2;
            vector<vector<int>> graph1, graph2;
            bool matrixCheckResult = false;

            graph1 = {
                // clang-format off
            {NO_EDGE, 1,       1      },
            {NO_EDGE, NO_EDGE, 2      },
            {NO_EDGE, NO_EDGE, NO_EDGE}
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

            g1 += g2;
            matrixCheckResult = checkMatrixes(graph1, graph2, g1.getGraph(), op);
            CHECK(matrixCheckResult == true);  // check if the adjacency matrix have the correct values
            CHECK(g1.getNumEdges() == 6);
            CHECK(g1.isDirectedGraph() == true);
        }
    }
}

TEST_CASE("-=") {
    function<int(int, int)> op = [](int a, int b) { return a - b; };
    Graph g1, g2;
    vector<vector<int>> graph1, graph2;
    bool matrixCheckResult = false;
    SUBCASE("undirected graph") {
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

            g1 -= g2;
            matrixCheckResult = checkMatrixes(graph1, graph2, g1.getGraph(), op);
            CHECK(matrixCheckResult == true);  // check if the adjacency matrix have the correct values
            CHECK(g1.isDirectedGraph() == false);
            CHECK(g1.getNumEdges() == 3);
            CHECK(g1.isHaveNegativeEdgeWeight() == false);
            CHECK(g1.isWeightedGraph() == false);
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

            g1 -= g2;
            matrixCheckResult = checkMatrixes(graph1, graph2, g1.getGraph(), op);

            CHECK(matrixCheckResult == true);               // check if the adjacency matrix have the correct values
            CHECK(g1.isHaveNegativeEdgeWeight() == false);  // check if the graph have negative edge
            CHECK(g1.isWeightedGraph() == false);           // check if the graph is weighted
            CHECK(g1.isDirectedGraph() == false);           // check if the graph is directed
            CHECK(g1.getNumEdges() == 0);                   // check if the number of edges is correct
        }
    }
    SUBCASE("directed graph") {
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

        g1 -= g2;

        matrixCheckResult = checkMatrixes(graph1, graph2, g1.getGraph(), op);
        CHECK(matrixCheckResult);  // check if the adjacency matrix have the correct values
        CHECK(g1.getNumEdges() == 4);
        CHECK(g1.isHaveNegativeEdgeWeight() == true);
    }
}

TEST_CASE("prefix ++") {
    Graph g1, g2;
    vector<vector<int>> graph, expected;
    SUBCASE("simple case") {
        graph = {
            // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        g2 = ++g1;

        expected = {
            // clang-format off
                {NO_EDGE, 2,       2      },
                {2,       NO_EDGE, 2      },
                {2,       2,       NO_EDGE}
            // clang-format on
        };

        CHECK(g1.getGraph() == g2.getGraph());    // check if the adjacency matrix have the same values
        CHECK(g2.getGraph() == expected);         // check if the adjacency matrix have the same values
        CHECK(&g1 != &g2);                        // check if the address is different
        CHECK(&g1.getGraph() != &g2.getGraph());  // check if the address of the adjacency matrix is different
        CHECK(g2.isDirectedGraph() == false);     // check if the graph is directed
    }
    SUBCASE("remove edges") {
        graph = {
            // clang-format off
                {NO_EDGE, 1,       -1     },
                {1,       NO_EDGE, 1      },
                {-1,       1,      NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        g2 = ++g1;

        expected = {
            // clang-format off
                {NO_EDGE, 2,       0      },
                {2,       NO_EDGE, 2      },
                {0,       2,       NO_EDGE}
            // clang-format on
        };

        CHECK(g1.getGraph() == g2.getGraph());    // check if the adjacency matrix have the same values
        CHECK(g2.getGraph() == expected);         // check if the adjacency matrix have the same values
        CHECK(&g1 != &g2);                        // check if the address is different
        CHECK(&g1.getGraph() != &g2.getGraph());  // check if the address of the adjacency matrix is different
        CHECK(g2.isDirectedGraph() == false);     // check if the graph is directed
        CHECK(g2.getNumEdges() == 2);             // check if the number of edges is correct
    }

    SUBCASE("empty graph") {
        graph = {
            // clang-format off
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        g2 = ++g1;

        expected = {
            // clang-format off
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE}
            // clang-format on
        };

        CHECK(g1.getGraph() == g2.getGraph());    // check if the adjacency matrix have the same values
        CHECK(g2.getGraph() == expected);         // check if the adjacency matrix have the same values
        CHECK(&g1 != &g2);                        // check if the address is different
        CHECK(&g1.getGraph() != &g2.getGraph());  // check if the address of the adjacency matrix is different
        CHECK(g2.isDirectedGraph() == false);     // check if the graph is directed
    }
}

TEST_CASE("postfix ++") {
    Graph g1, g2;
    vector<vector<int>> graph, expected_g1, expected_g2;
    SUBCASE("simple case") {
        graph = {
            // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        g2 = g1++;
        expected_g1 = {
            // clang-format off
                {NO_EDGE, 2,       2      },
                {2,       NO_EDGE, 2      },
                {2,       2,       NO_EDGE}
            // clang-format on
        };

        expected_g2 = {
            // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
            // clang-format on
        };

        CHECK(g1.getGraph() == expected_g1);  // check if the adjacency matrix have the same values
        CHECK(g2.getGraph() == expected_g2);  // check if the adjacency matrix have the same values
        CHECK(&g1 != &g2);                    // check if the address is different
    }
    SUBCASE("remove edges") {
        graph = {
            // clang-format off
                {NO_EDGE, 1,       -1     },
                {1,       NO_EDGE, 1      },
                {-1,       1,      NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        g2 = g1++;

        expected_g1 = {
            // clang-format off
                {NO_EDGE, 2,       0      },
                {2,       NO_EDGE, 2      },
                {0,       2,       NO_EDGE}
            // clang-format on
        };

        expected_g2 = {
            // clang-format off
                {NO_EDGE, 1,       -1     },
                {1,       NO_EDGE, 1      },
                {-1,       1,      NO_EDGE}
            // clang-format on
        };

        CHECK(g1.getGraph() == expected_g1);  // check if the adjacency matrix have the same values
        CHECK(g2.getGraph() == expected_g2);  // check if the adjacency matrix have the same values
        CHECK(&g1 != &g2);                    // check if the address is different
        CHECK(g1.getNumEdges() == 2);         // check if the number of edges is correct
    }

    SUBCASE("empty graph") {
        graph = {
            // clang-format off
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        g2 = g1++;

        vector<vector<int>> expected = {
            // clang-format off
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE}
            // clang-format on
        };

        CHECK(g1.getGraph() == expected);  // check if the adjacency matrix have the same values
        CHECK(g2.getGraph() == expected);  // check if the adjacency matrix have the same values
        CHECK(&g1 != &g2);                 // check if the address is different
    }
}

TEST_CASE("prefix --") {
    SUBCASE("simple case") {
        Graph g1;
        vector<vector<int>> graph = {
            // clang-format off
                {NO_EDGE, NO_EDGE, 2      },
                {NO_EDGE, NO_EDGE, 1      },
                {3,       1,       NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        Graph g2 = --g1;

        vector<vector<int>> expected = {
            // clang-format off
                {NO_EDGE, NO_EDGE, 1      },
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {2,       NO_EDGE, NO_EDGE}
            // clang-format on
        };

        CHECK(g1.getGraph() == g2.getGraph());    // check if the adjacency matrix have the same values
        CHECK(g2.getGraph() == expected);         // check if the adjacency matrix have the same values
        CHECK(&g1 != &g2);                        // check if the address is different
        CHECK(&g1.getGraph() != &g2.getGraph());  // check if the address of the adjacency matrix is different
        CHECK(g2.isDirectedGraph() == true);      // check if the graph is directed
        CHECK(g2.getNumEdges() == 2);             // check if the number of edges is correct
    }
    SUBCASE("remove edges") {
        Graph g1;
        vector<vector<int>> graph = {
            // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        Graph g2 = --g1;

        vector<vector<int>> expected = {
            // clang-format off
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE}
            // clang-format on
        };

        CHECK(g1.getGraph() == g2.getGraph());    // check if the adjacency matrix have the same values
        CHECK(g2.getGraph() == expected);         // check if the adjacency matrix have the same values
        CHECK(&g1 != &g2);                        // check if the address is different
        CHECK(&g1.getGraph() != &g2.getGraph());  // check if the address of the adjacency matrix is different
        CHECK(g2.isDirectedGraph() == false);     // check if the graph is directed
        CHECK(g2.getNumEdges() == 0);             // check if the number of edges is correct
    }
}

TEST_CASE("postfix --") {
    Graph g1;
    vector<vector<int>> graph = {
        // clang-format off
                {NO_EDGE, NO_EDGE, 2      },
                {NO_EDGE, NO_EDGE, 1      },
                {3,       1,       NO_EDGE}
        // clang-format on
    };
    g1.loadGraph(graph);

    Graph g2 = g1--;

    vector<vector<int>> expected_g1 = {
        // clang-format off
                {NO_EDGE, NO_EDGE, 1      },
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {2,       NO_EDGE, NO_EDGE}
        // clang-format on
    };

    CHECK(g1.getGraph() == expected_g1);  // check if the adjacency matrix have the same values
    CHECK(g2.getGraph() == graph);        // check if the adjacency matrix have the same values
    CHECK(&g1 != &g2);                    // check if the address is different
}

TEST_CASE("*") {
    SUBCASE("Graph * Graph") {
        SUBCASE("undirected graph") {}
        SUBCASE("directed graph") {}
    }

    SUBCASE("(Graph * int) and (int * Graph)") {
        Graph g1;
        vector<vector<int>> graph = {
            // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        SUBCASE("Graph * int") {
            Graph g2 = g1 * 2;

            vector<vector<int>> expected = {
                // clang-format off
                {NO_EDGE, 2,       2      },
                {2,       NO_EDGE, 2      },
                {2,       2,       NO_EDGE}
                // clang-format on
            };

            CHECK(g2.getGraph() == expected);  // check if the adjacency matrix have the same values

            graph = {
                // clang-format off
                {NO_EDGE, 1,       1      },
                {NO_EDGE, NO_EDGE, 2      },
                {NO_EDGE, 5,       NO_EDGE}
                // clang-format on
            };
            g1.loadGraph(graph);

            g2 = g1 * 2;

            expected = {
                // clang-format off
                {NO_EDGE, 2,       2      },
                {NO_EDGE, NO_EDGE, 4      },
                {NO_EDGE, 10,      NO_EDGE}
                // clang-format on
            };

            CHECK(g2.getGraph() == expected);  // check if the adjacency matrix have the same values
        }

        SUBCASE("int * Graph") {
            Graph g2 = 2 * g1;

            vector<vector<int>> expected = {
                // clang-format off
                {NO_EDGE, 2,       2      },
                {2,       NO_EDGE, 2      },
                {2,       2,       NO_EDGE}
                // clang-format on
            };

            CHECK(g2.getGraph() == expected);  // check if the adjacency matrix have the same values
        }

        SUBCASE("Graph * 0") {
            Graph g2 = g1 * 0;

            vector<vector<int>> expected = {
                // clang-format off
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE}
                // clang-format on
            };

            CHECK(g2.getGraph() == expected);  // check if the adjacency matrix have the same values
        }
    }
}

TEST_CASE("*=") {
    SUBCASE("Graph * Graph") {
        SUBCASE("undirected graph") {}
        SUBCASE("directed graph") {}
    }

    SUBCASE("Graph * int") {
        Graph g1;
        vector<vector<int>> graph = {
            // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        g1 *= 2;

        vector<vector<int>> expected = {
            // clang-format off
            {NO_EDGE, 2,       2      },
            {2,       NO_EDGE, 2      },
            {2,       2,       NO_EDGE}
            // clang-format on
        };

        CHECK(g1.getGraph() == expected);  // check if the adjacency matrix have the same values

        graph = {
            // clang-format off
            {NO_EDGE, 1,       1      },
            {NO_EDGE, NO_EDGE, 2      },
            {NO_EDGE, 5,       NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        g1 *= 2;

        expected = {
            // clang-format off
            {NO_EDGE, 2,       2      },
            {NO_EDGE, NO_EDGE, 4      },
            {NO_EDGE, 10,      NO_EDGE}
            // clang-format on
        };

        CHECK(g1.getGraph() == expected);  // check if the adjacency matrix have the same values
    }
}

TEST_CASE("==") {
    SUBCASE("undirected graph") {
        SUBCASE("simple equal graphs") {
            Graph g1, g2;
            vector<vector<int>> graph = {
                // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
                // clang-format on
            };
            g1.loadGraph(graph);

            vector<vector<int>> graph2 = {
                // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
                // clang-format on
            };

            g2.loadGraph(graph2);

            CHECK(g1 == g2);
        }

        SUBCASE("empty graphs") {
            Graph g1, g2;
            vector<vector<int>> graph = {
                // clang-format off
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE}
                // clang-format on
            };
            g1.loadGraph(graph);

            vector<vector<int>> graph2 = {
                // clang-format off
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {NO_EDGE, NO_EDGE, NO_EDGE}
                // clang-format on
            };

            g2.loadGraph(graph2);

            CHECK(g1 == g2);

            graph = {};
            graph2 = {};
            g1.loadGraph(graph);
            g2.loadGraph(graph2);

            CHECK(g1 == g2);
        }

        SUBCASE("different adjacency matrixes, but still equal") {
            Graph g1, g2;
            vector<vector<int>> graph = {
                // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, -1      },
                {1,       -1,       NO_EDGE}
                // clang-format on
            };
            g1.loadGraph(graph);

            vector<vector<int>> graph2 = {
                // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
                // clang-format on
            };

            g2.loadGraph(graph2);

            CHECK(g1 == g2);
        }

        SUBCASE("different graphs") {
            // the number of edges is different
            Graph g1, g2;
            vector<vector<int>> graph = {
                // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
                // clang-format on
            };

            g1.loadGraph(graph);

            vector<vector<int>> graph2 = {
                // clang-format off
                {NO_EDGE, 1      , NO_EDGE},
                {1,       NO_EDGE, 1      },
                {NO_EDGE, 1      , NO_EDGE}
                // clang-format on
            };

            g2.loadGraph(graph2);

            CHECK_FALSE(g1 == g2);
            CHECK(g1 != g2);
        }
    }
    SUBCASE("directed graph") {}
}

TEST_CASE("!=") {
    SUBCASE("undirected graph") {}
    SUBCASE("directed graph") {}
}

TEST_CASE("<") {
    SUBCASE("undirected graph") {
        SUBCASE("G1 in subset of G2") {
            Graph g1, g2;
            vector<vector<int>> graph = {
                // clang-format off
                {NO_EDGE, 1,       NO_EDGE },
                {1,       NO_EDGE, 1      },
                {NO_EDGE, 1,       NO_EDGE}
                // clang-format on
            };
            g1.loadGraph(graph);

            vector<vector<int>> graph2 = {
                // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
                // clang-format on
            };

            g2.loadGraph(graph2);

            CHECK(g1 < g2);
        }

        SUBCASE("G1 equals G2") {
            Graph g1, g2;
            vector<vector<int>> graph = {
                // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
                // clang-format on
            };
            g1.loadGraph(graph);

            vector<vector<int>> graph2 = {
                // clang-format off
                {NO_EDGE, 1,       1      },
                {1,       NO_EDGE, 1      },
                {1,       1,       NO_EDGE}
                // clang-format on
            };

            g2.loadGraph(graph2);

            CHECK_FALSE(g1 < g2);
            CHECK_FALSE(g2 < g1);
        }

        SUBCASE("G1 not in subset of G2 and G2 have more edges") {
            Graph g1, g2;
            vector<vector<int>> graph = {
                // clang-format off
                {NO_EDGE, NO_EDGE, 1      },
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {1,       NO_EDGE, NO_EDGE}
                // clang-format on
            };
            g1.loadGraph(graph);

            vector<vector<int>> graph2 = {
                // clang-format off
                {NO_EDGE, 1      , NO_EDGE},
                {1,       NO_EDGE, 1      },
                {NO_EDGE, 1      , NO_EDGE}
                // clang-format on
            };

            g2.loadGraph(graph2);

            CHECK(g1 < g2);
            CHECK_FALSE(g2 < g1);
        }

        SUBCASE("G1 not in subset of G2 and |E(G1)| = |E(G2)| and |V(G1)|<|V(G2)|") {
            Graph g1, g2;
            vector<vector<int>> graph = {
                // clang-format off
                {NO_EDGE, NO_EDGE, 1      },
                {NO_EDGE, NO_EDGE, NO_EDGE},
                {1,       NO_EDGE, NO_EDGE}
                // clang-format on
            };
            g1.loadGraph(graph);

            vector<vector<int>> graph2 = {
                // clang-format off
                {NO_EDGE, 1      , NO_EDGE, 1      },
                {1,       NO_EDGE, 1      , 1      },
                {NO_EDGE, 1      , NO_EDGE, 1      },
                {1,       1      , 1      , NO_EDGE}
                // clang-format on
            };

            g2.loadGraph(graph2);

            CHECK(g1 < g2);
            CHECK_FALSE(g2 < g1);
        }
    }
    SUBCASE("directed graph") {}
}

TEST_CASE("<<") {
    SUBCASE("simple case") {
        Graph g1;
        vector<vector<int>> graph = {
            // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        stringstream ss;
        ss << g1;

        string expected = "[X, 1, 1],\n[1, X, 1],\n[1, 1, X]";

        CHECK(ss.str() == expected);
    }

    SUBCASE("empty graph") {
        Graph g1;
        vector<vector<int>> graph = {
            // clang-format off
            {NO_EDGE, NO_EDGE, NO_EDGE},
            {NO_EDGE, NO_EDGE, NO_EDGE},
            {NO_EDGE, NO_EDGE, NO_EDGE}
            // clang-format on
        };
        g1.loadGraph(graph);

        stringstream ss;
        ss << g1;

        string expected = "[X, X, X],\n[X, X, X],\n[X, X, X]";

        CHECK(ss.str() == expected);
    }
}