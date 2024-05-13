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

TEST_CASE("unary +") {
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
    CHECK(g1.getGraph() == g2.getGraph());  // check if the adjacency matrix have the same values
    CHECK(&g1 != &g2);                      // check if the address is different
}

TEST_CASE("unary -") {
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
    CHECK(&g1 != &g2);  // check if the address is different
}

TEST_CASE("Binary +") {
    Graph g1(false);
    vector<vector<int>> graph1 = {
        // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
        // clang-format on
    };
    g1.loadGraph(graph1);

    Graph g2(false);
    vector<vector<int>> graph2 = {
        // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
        // clang-format on
    };
    g2.loadGraph(graph2);

    Graph g3 = g1 + g2;
    for (size_t i = 0; i < graph1.size(); i++) {
        for (size_t j = 0; j < graph1[i].size(); j++) {
            if (graph1[i][j] == NO_EDGE || graph2[i][j] == NO_EDGE)
                CHECK(g3.getGraph()[i][j] == NO_EDGE);  // check if the edge is NO_EDGE
            else if (graph1[i][j] + graph2[i][j] == 0) {
                CHECK(g3.getGraph()[i][j] == NO_EDGE);
            } else
                CHECK(g3.getGraph()[i][j] == graph1[i][j] + graph2[i][j]);  // check if the edge is the sum of the original edges
        }
    }
    CHECK(&g1 != &g3);  // check if the address is different
    CHECK(&g2 != &g3);  // check if the address is different
}