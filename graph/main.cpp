#include <iostream>
#include <vector>

#include "Graph.hpp"

using namespace std;
using namespace shayg;

int main() {
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

    cout << g1 << endl;
    cout << g2 << endl;
    cout << g3 << endl;
    
    return 0;
}