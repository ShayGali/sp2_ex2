#include <iostream>
#include <vector>

#include "Graph.hpp"

using namespace std;
using namespace shayg;

int main() {
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
    
    cout << g1 << endl;
    cout << g2 << endl;

    cout << (g1 < g2) << endl;
    cout << (g2 < g1) << endl;
    cout << (g1 == g2) << endl;
    return 0;
}