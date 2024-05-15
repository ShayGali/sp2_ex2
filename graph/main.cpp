#include <iostream>
#include <vector>

#include "Graph.hpp"

using namespace std;
using namespace shayg;

int main() {
    Graph g1(false), g2(false), g3(false);
    vector<vector<int>> graph1, graph2;

    graph1 = {
        // clang-format off
            {NO_EDGE, NO_EDGE, NO_EDGE},
            {NO_EDGE, NO_EDGE, NO_EDGE},
            {NO_EDGE, NO_EDGE, NO_EDGE}
        // clang-format on
    };
    g1.loadGraph(graph1);

    cout << g1 << endl;

    // g1.printGraph();
    // graph2 = {
    //     // clang-format off
    //         {NO_EDGE, -1,       NO_EDGE},
    //         {-1,       NO_EDGE, NO_EDGE},
    //         {NO_EDGE, NO_EDGE, NO_EDGE}
    //     // clang-format on
    // };
    // g2.loadGraph(graph2);
    // cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    // g3 = g1 + g2;

    // cout << g3 << endl;
    return 0;
}