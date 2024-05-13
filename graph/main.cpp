#include <iostream>
#include <vector>

#include "Graph.hpp"

using namespace std;
using namespace shayg;

int main() {
    Graph g1(false);
    vector<vector<int>> graph = {
        // clang-format off
            {NO_EDGE, 1,       1      },
            {1,       NO_EDGE, 1      },
            {1,       1,       NO_EDGE}
        // clang-format on
    };

    g1.loadGraph(graph);
    return 0;
}