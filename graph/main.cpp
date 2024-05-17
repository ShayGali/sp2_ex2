#include <iostream>
#include <vector>

#include "Graph.hpp"

using namespace std;
using namespace shayg;

int main() {
    Graph g1, g2, g3;
    vector<vector<int>> graph1, graph2, graph3;
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

    g1.loadGraph(graph1);
    g2.loadGraph(graph2);

    g3 = g1 + g2;

    cout << g3.isDirectedGraph() << endl;
    cout << g3.getNumEdges() << endl;
    cout << g3 << endl;

    return 0;
}